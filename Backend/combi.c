#include "combi.h"
#include "genericNode.h"
#include "RNGs.h"


void combiNode( const MPI_Comm commNodes,  const  Combi *initialStatus, const int mpiProcesses){
	//variables a ser enviadas al printer
	PrinterActivity cReport;

	int inputWorktask = 0;//que estan en la entrada antes del cuerpo
	int outputWorktask = 0; //que cumplieron el dalay se se pueden ir
	int bodyResource = 0; //counterWorkTask
	Worktask *workTaskList;
    workTaskList = (Worktask *)malloc(sizeof(Worktask)); //nodo dummy de workstasks
    workTaskList -> next = NULL;
	int msg = 0;

	//???
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo
	
	RngInstance rngProbabilisticBranch;
	rngProbabilisticBranch.isInitialise = FALSE;
	RngInstance rngDrawn;
	rngDrawn.isInitialise = FALSE;

	if(initialStatus->delay.distribution != DIST_DETERMINISTIC &&  initialStatus->delay.seed != -1){
		RandomInitialise(&rngDrawn, initialStatus->delay.seed, initialStatus->delay.seed);
	}

	if(initialStatus->countProbabilisticBranch > 0 && initialStatus->modelSeed > -1){
		RandomInitialise(&rngProbabilisticBranch, initialStatus->modelSeed,initialStatus->modelSeed);
	}
	
	cReport.idNode = initialStatus->idNode;
	cReport.counterInput = 0 ;
	cReport.amountDelay = 0.0;
	cReport.maximunDrawn = NAN; //NAN definido en math.h
	cReport.minimunDrawn = NAN;
	
	do {
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				advancePhaseCombi( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, FALSE, &rngProbabilisticBranch );
				break;
			case ADVANCE_PAHSE_PRIMA:
				advancePhaseCombi( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, TRUE, &rngProbabilisticBranch );
				break;
			case GENERATION_PHASE: //hace lo mismo que la de abajo
			case GENERATION_PHASE_PRIMA:
				generationPhaseCombi( &inputWorktask, &bodyResource, &outputWorktask, commNodes, workTaskList, initialStatus, &cReport, &rngDrawn);
				break;
			case CONSUME_DT:
				deltaTCount++;
				int goOut = discountDelayAndDeleteFinishedWorktask(workTaskList);
				bodyResource -= goOut;
				outputWorktask += goOut;
				MPI_Barrier( commNodes );
				break;
			case PING_REPORT:
				cReport.activityInside = bodyResource;
				MPI_Send(&cReport, sizeof(PrinterActivity), MPI_BYTE, PRINTER_ID, COMBI_REPORT , MPI_COMM_WORLD);
				if(bodyResource > 0){
					double* worktask = delayOfWorktask(workTaskList, bodyResource);
					MPI_Send(worktask, cReport.activityInside* 2, MPI_DOUBLE, PRINTER_ID, COMBI_REPORT , MPI_COMM_WORLD);
					free(worktask);
				}
			default:
				break;
		}
	} while (msg != LIVE_LOCK);
	return;
}

void advancePhaseCombi(int * inputWorktask, int* outputWorktask, const Combi *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, RngInstance* rngProbabilisticBranch ){ 
    for(;;){
    	if(!hasQueueResources( initialStatus, commNodes)){
	    	resourcesNoDemand( initialStatus, commNodes);
	    	resourcesSend( initialStatus, commNodes, outputWorktask, rngProbabilisticBranch );
	    	finishCombi( isPrima, commNodes , inputWorktask, mpiProcesses );
	    	break;
    	}else{
	    	resourcesDemand(initialStatus, commNodes);
	    	if(allTransactionBegin( commNodes ,initialStatus )){
	    		setAllCommit(initialStatus, commNodes );
	    		(*inputWorktask)++;
	    	} else {
	    		setAllRollback( initialStatus, commNodes );
	    		resourcesSend( initialStatus, commNodes, outputWorktask,rngProbabilisticBranch );
	    		finishCombi( isPrima,  commNodes , inputWorktask, mpiProcesses );
	    		break;
	    	}
    	}
    }
	return;
}

//Espero a que todas las combis me manden resource request
int hasQueueResources( const Combi *initialStatus, const MPI_Comm commNodes){
	int allHas = TRUE;
	int msg;
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_REQUEST, commNodes);
		MPI_Recv( &msg, 1, MPI_INT, initialStatus->preceders[i], RESOURCE_RESPONSE, commNodes, MPI_STATUS_IGNORE);
		allHas &= (msg)?TRUE:FALSE;
	}
	return allHas;
}

//Envio resource no demand a las colas
void resourcesNoDemand( const Combi *initialStatus, const MPI_Comm commNodes){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_NO_DEMAND, commNodes);
	}
	return;
}

//Envio resource no demand a las colas
void resourcesDemand( const Combi *initialStatus, const MPI_Comm commNodes){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_DEMAND, commNodes);
	}
	return;
}

//envio resource send a los followers
void resourcesSend( const Combi *initialStatus, const MPI_Comm commNodes, int* worktaskInOutput, RngInstance* rngProbabilisticBranch ){
	
	double* walls = NULL;
	int* hollows = NULL;
	int coins = (*worktaskInOutput);
	if (initialStatus->countProbabilisticBranch > 0){
		walls = (double*) malloc(initialStatus->countProbabilisticBranch * sizeof(double));
		hollows = (int*) malloc(initialStatus->countProbabilisticBranch * sizeof(int)) ;
		hollows[initialStatus->countProbabilisticBranch-1] = 0; //inicializo de paso
		walls[ initialStatus->countProbabilisticBranch -1] = 1.0; 
	}
	double acummulatedProb = 0.0;
	for(int i = 0; i < initialStatus->countProbabilisticBranch-1; i++){ 
		acummulatedProb += initialStatus->probabilisticBranch[i];
		walls[i] = acummulatedProb;
		hollows[i] = 0; //inicializo de paso
	}
	
	//para cada nodo sortear	
	for(int i = 0; i < initialStatus->countProbabilisticBranch; i++){
		if( coins ){
			double hollowNumber = RandomUniform(rngProbabilisticBranch);
			//defino donde cae la moneda
			for(int j = 0; j < initialStatus->countProbabilisticBranch; j++){
				if( hollowNumber <= walls[j] ){
					hollows[j]++;
					coins--;
					break;
				}
			}
		} else {
			break;
		}
		
	}
	 
    //inicializo los request de las llegadas de recursos
    MPI_Request* requestFollowers = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countFollowers);
    
    if (initialStatus->countProbabilisticBranch == 0){
		//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
		for (int i = 0 ; i < initialStatus->countFollowers; i++){
			 MPI_Isend( worktaskInOutput, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
		}
	} else {
		for (int i = 0 ; i < initialStatus->countFollowers; i++){
			 MPI_Isend( &hollows[i], 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
		}
	}
	
	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		MPI_Wait(&requestFollowers[i], MPI_STATUS_IGNORE);
		(*worktaskInOutput) = 0;
	}
	free(requestFollowers);
	if(walls)free(walls);
	if(hollows)free(hollows);
}


void finishCombi(const int isPrima, const MPI_Comm commNodes ,const int* inputResource, const int mpiProcesses ){
	int msg;
	 if( !isPrima ){
		MPI_Barrier( commNodes );
	} else {
		int * nodesStatus = NULL;
		msg = (*inputResource)? FALSE: TRUE;
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
	}

}

int allTransactionBegin(const MPI_Comm commNodes ,const Combi *initialStatus ){
	int msg;
	int allBegin = TRUE;
	MPI_Status infoComm;
	int currentTag;
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Recv( &msg, 1, MPI_INT, initialStatus->preceders[i], MPI_ANY_TAG, commNodes, &infoComm);
		currentTag = infoComm.MPI_TAG;
		allBegin  &= (currentTag == TRANSACTION_BEGIN)?TRUE:FALSE;
	}
	return allBegin;
}

void setAllRollback(const Combi *initialStatus, const MPI_Comm commNodes ){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], TRANSACTION_ROLLBACK, commNodes);
	}
	return ;
}

void setAllCommit(const Combi *initialStatus, const MPI_Comm commNodes ){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], TRANSACTION_COMMIT, commNodes);
	}
	return ;
}

void generationPhaseCombi(int* inputWorktask, int* bodyResource, int* outputWorktask, const MPI_Comm commNodes, Worktask *workTaskList,  const Combi *initialStatus, PrinterActivity* cReport, RngInstance* rngDrawn){
	cReport-> counterInput += (*inputWorktask);
	double humanDelay = 0.0;
	//TODO ARmar wraper de generador que recibe daly como unico argumento y llamar a un puntero a funcion
	switch(initialStatus->delay.distribution){
		case DIST_DETERMINISTIC:
			for(int i = 0; i < (*inputWorktask); i++){
				humanDelay = initialStatus->delay.constant;
				setNewWorktask(workTaskList, humanDelay, cReport);
			}
		break;

		case DIST_UNIFORM:
        	for(int i = 0; i < (*inputWorktask); i++){
        		humanDelay = RandomDouble(rngDrawn, initialStatus->delay.least, initialStatus->delay.highest);
				setNewWorktask(workTaskList, humanDelay, cReport);
			}
		break;

		case DIST_NORMAL:
		
    		for(int i = 0; i < (*inputWorktask); i++){
    			humanDelay = RandomNormal(rngDrawn,initialStatus->delay.mean, initialStatus->delay.variance );
				setNewWorktask(workTaskList, humanDelay, cReport);
			}
		break;

		case DIST_EXPONENTIAL:
			
     		for(int i = 0; i < (*inputWorktask); i++){
     			humanDelay = RandomExponential(rngDrawn, initialStatus->delay.lambda );
     			setNewWorktask(workTaskList, humanDelay, cReport);
			}
		break;

		case DIST_TRIANGULAR:
			
			for(int i = 0; i < (*inputWorktask); i++){
				humanDelay = RandomTriangular(rngDrawn, initialStatus->delay.least, initialStatus->delay.highest, initialStatus->delay.mode);
				setNewWorktask(workTaskList, humanDelay, cReport);
			}
		break;

		case DIST_LOG_NORMAL:
			
			for(int i = 0; i < (*inputWorktask); i++){
				humanDelay =  RandomLogNormalWithMinimun(rngDrawn, initialStatus->delay.escale, initialStatus->delay.shape, initialStatus->delay.least );
				setNewWorktask(workTaskList, humanDelay, cReport);
			}
		break;

		case DIST_BETA:
		
			for(int i = 0; i < (*inputWorktask); i++){
				humanDelay = RandomBeta( rngDrawn, initialStatus->delay.shapeAlpha, initialStatus->delay.shapeBeta, initialStatus->delay.minimun, initialStatus->delay.maximun );
				setNewWorktask(workTaskList, humanDelay, cReport);
			}
			
		break;

	}

	(*bodyResource) += (*inputWorktask);
	(*inputWorktask) = 0;

	int detZero = deleteFinishedWorktask(workTaskList);
	(*bodyResource) -= detZero;
	(*outputWorktask) += detZero;

	MPI_Barrier( commNodes );
}
