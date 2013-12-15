#include "combi.h"
#include "genericNode.h"
#include "RNGs.h"


void combiNode( const MPI_Comm commNodes,  const  Combi *initialStatus, const int mpiProcesses, const int modelSeed){
	char* fileName = NULL;
	int len = snprintf(NULL, 0, "/tmp/combi.%d.log",initialStatus->idNode);
	fileName = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(fileName, (len + 1), "/tmp/combi.%d.log",initialStatus->idNode);
	
	int fileDescriptor = open (fileName, O_WRONLY|O_CREAT|O_TRUNC, 00660);
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

	if(initialStatus->countProbabilisticBranch > 0 && modelSeed > -1){
		RandomInitialise(&rngProbabilisticBranch, modelSeed,modelSeed);
	}
	
	cReport.idNode = initialStatus->idNode;
	cReport.counterInput = 0 ;
	cReport.amountDelay = 0.0;
	cReport.maximunDrawn = NAN; //NAN definido en math.h
	cReport.minimunDrawn = NAN;
	
	loger( fileDescriptor, "-- Inicio --\n");
	do {
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				logPhase(fileDescriptor,"Avance Phase input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				advancePhaseCombi( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, FALSE, modelSeed,&rngProbabilisticBranch,fileDescriptor);
				logPhase(fileDescriptor,"End Avance Phase input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				break;
			case ADVANCE_PAHSE_PRIMA:
				logPhase(fileDescriptor,"Avance Phase Prima input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				advancePhaseCombi( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, TRUE, modelSeed, &rngProbabilisticBranch,fileDescriptor);
				logPhase(fileDescriptor,"End Avance Phase Prima input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				break;
			case GENERATION_PHASE: //hace lo mismo que la de abajo
			case GENERATION_PHASE_PRIMA:
				logPhase(fileDescriptor,"Generation Phase/Generation Phase Prima, input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				generationPhaseCombi( &inputWorktask, &bodyResource, &outputWorktask, commNodes, workTaskList, initialStatus, &cReport, &rngDrawn);
				logPhase(fileDescriptor,"End Generation Phase/Generation Phase Prima, input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				break;
			case CONSUME_DT:
				logPhase(fileDescriptor,"CONSUME_DT, input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				deltaTCount++;
				int goOut = discountDelayAndDeleteFinishedWorktask(workTaskList);
				bodyResource -= goOut;
				outputWorktask += goOut;
				MPI_Barrier( commNodes );
				logPhase(fileDescriptor,"END CONSUME_DT, input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				break;
			case PING_REPORT:
				logPhase(fileDescriptor,"PING_REPORT, input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
				cReport.activityInside = bodyResource;
				MPI_Send(&cReport, sizeof(PrinterActivity), MPI_BYTE, PRINTER_ID, COMBI_REPORT , MPI_COMM_WORLD);
				if(bodyResource > 0){
					double* worktask = delayOfWorktask(workTaskList, bodyResource);
					MPI_Send(worktask, cReport.activityInside* 2, MPI_DOUBLE, PRINTER_ID, COMBI_REPORT , MPI_COMM_WORLD);
					free(worktask);
				}
				logPhase(fileDescriptor,"END PING_REPORT, input = %d, body = %d , output = %d\n",inputWorktask,bodyResource,outputWorktask);
			default:
				break;
		}
	} while (msg != LIVE_LOCK);
	loger( fileDescriptor, "-- LIVE_LOCK --\n");
	close(fileDescriptor);
	return;
}

void advancePhaseCombi(int * inputWorktask, int* outputWorktask, const Combi *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, const int modelSeed, RngInstance* rngProbabilisticBranch, const int fileDescriptor){ 
    for(;;){
    	if(!hasQueueResources( initialStatus, commNodes,fileDescriptor)){
	    	resourcesNoDemand( initialStatus, commNodes,fileDescriptor);
	    	resourcesSend( initialStatus, commNodes, outputWorktask, modelSeed, rngProbabilisticBranch,fileDescriptor);
	    	finishCombi( isPrima, commNodes , inputWorktask, mpiProcesses,fileDescriptor);
	    	break;
    	}else{
	    	resourcesDemand(initialStatus, commNodes,fileDescriptor);
	    	if(allTransactionBegin( commNodes ,initialStatus,fileDescriptor)){
	    		setAllCommit(initialStatus, commNodes,fileDescriptor);
	    		(*inputWorktask)++;
	    	} else {
	    		setAllRollback( initialStatus, commNodes,fileDescriptor);
	    		resourcesSend( initialStatus, commNodes, outputWorktask, modelSeed,rngProbabilisticBranch, fileDescriptor);
	    		finishCombi( isPrima,  commNodes , inputWorktask, mpiProcesses,fileDescriptor);
	    		break;
	    	}
    	}
    }
	return;
}

//Espero a que todas las combis me manden resource request
int hasQueueResources( const Combi *initialStatus, const MPI_Comm commNodes, const int fileDescriptor){
	int allHas = TRUE;
	int msg;
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_REQUEST, commNodes);
		loger( fileDescriptor, ">>> RESOURCE_REQUEST >>>\n");
		MPI_Recv( &msg, 1, MPI_INT, initialStatus->preceders[i], RESOURCE_RESPONSE, commNodes, MPI_STATUS_IGNORE);
		logPhase(fileDescriptor,"<<< Q%d = %d <<< RESOURCE_RESPONSE ( %d )<<< \n",i,initialStatus->preceders[i],msg);
		allHas &= (msg)?TRUE:FALSE;
	}
	return allHas;
}

//Envio resource no demand a las colas
void resourcesNoDemand( const Combi *initialStatus, const MPI_Comm commNodes, const int fileDescriptor){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_NO_DEMAND, commNodes);
		logPhase(fileDescriptor,"%d >>> RESOURCE_NO_DEMAND >>> Q%d = %d \n",0,i,initialStatus->preceders[i]);
	}
	return;
}

//Envio resource no demand a las colas
void resourcesDemand( const Combi *initialStatus, const MPI_Comm commNodes, const int fileDescriptor){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_DEMAND, commNodes);
		logPhase(fileDescriptor,"%d >>> RESOURCE_DEMAND >>> Q%d = %d \n",0,i,initialStatus->preceders[i]);
	}
	return;
}

//envio resource send a los followers
void resourcesSend( const Combi *initialStatus, const MPI_Comm commNodes, int* worktaskInOutput, const int modelSeed, RngInstance* rngProbabilisticBranch , const int fileDescriptor){
	
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
			 logPhase(fileDescriptor,">>> RESOURCE_SEND = %d >>> N%d=%d \n", (*worktaskInOutput) , i,  initialStatus->followers[i]);
		}
	} else {
		for (int i = 0 ; i < initialStatus->countFollowers; i++){
			 MPI_Isend( &hollows[i], 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
			 logPhase(fileDescriptor,">>> RESOURCE_SEND = %d >>> N%d=%d \n", hollows[i] , i,  initialStatus->followers[i]);
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


void finishCombi(const int isPrima, const MPI_Comm commNodes ,const int* inputResource, const int mpiProcesses, const int fileDescriptor){
	int msg;
	 if( !isPrima ){
		MPI_Barrier( commNodes );
	} else {
		int * nodesStatus = NULL;
		msg = (*inputResource)? FALSE: TRUE;
		loger( fileDescriptor, (*inputResource)?">>> NOT TERMINATED NODE >>>\n":">>> TERMINATED NODE >>>\n");
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
	}

}

int allTransactionBegin(const MPI_Comm commNodes ,const Combi *initialStatus, const int fileDescriptor){
	int msg;
	int allBegin = TRUE;
	MPI_Status infoComm;
	int currentTag;
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Recv( &msg, 1, MPI_INT, initialStatus->preceders[i], MPI_ANY_TAG, commNodes, &infoComm);
		currentTag = infoComm.MPI_TAG;
		allBegin  &= (currentTag == TRANSACTION_BEGIN)?TRUE:FALSE;
		loger( fileDescriptor, (currentTag == TRANSACTION_BEGIN)?"<<< TRANSACTION_BEGIN <<<\n":"<<< TRANSACTION_CANCELED <<<\n");
	}
	return allBegin;
}

void setAllRollback(const Combi *initialStatus, const MPI_Comm commNodes, const int fileDescriptor){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], TRANSACTION_ROLLBACK, commNodes);
		loger( fileDescriptor, ">>> TRANSACTION_ROLLBACK >>>\n");
	}
	return ;
}

void setAllCommit(const Combi *initialStatus, const MPI_Comm commNodes, const int fileDescriptor){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], TRANSACTION_COMMIT, commNodes);
		loger( fileDescriptor, ">>> TRANSACTION_COMMIT >>>\n");
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
			//TODO : falta completar el caso de que solo uno de los dos tiene parametro mayor a uno
			if(initialStatus->delay.shapeAlpha < 1 && initialStatus->delay.shapeBeta < 1){
				for(int i = 0; i < (*inputWorktask); i++){
					humanDelay = RandomBetaWithMinimunAndMaximun( rngDrawn, initialStatus->delay.shapeAlpha, initialStatus->delay.shapeBeta, initialStatus->delay.minimun, initialStatus->delay.maximun );
					setNewWorktask(workTaskList, humanDelay, cReport);
				}
			} else {
				for(int i = 0; i < (*inputWorktask); i++){
					humanDelay = RandomBetaIntegerWithMinimunAndMaximun( rngDrawn, (int)initialStatus->delay.shapeAlpha, (int)initialStatus->delay.shapeBeta, initialStatus->delay.minimun, initialStatus->delay.maximun );
					setNewWorktask(workTaskList, humanDelay, cReport);
				}	
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
