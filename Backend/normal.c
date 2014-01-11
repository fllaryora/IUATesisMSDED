#include "normal.h"
#include "genericNode.h"
#include "RNGs.h"

void normalNode( const MPI_Comm commNodes,  const  Normal *initialStatus, const int mpiProcesses){
	PrinterActivity nReport;

	int inputWorktask = 0;//que estan en la entrada antes del cuerpo
	int outputWorktask = 0; //que cumplieron el dalay se se pueden ir
	int bodyResource = 0; //counterWorkTask
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo
	Worktask *workTaskList;
    workTaskList = (Worktask *)malloc(sizeof(Worktask)); //nodo dummy de workstasks
    workTaskList -> next = NULL;
   

	RngInstance rngProbabilisticBranch;
	rngProbabilisticBranch.isInitialise = FALSE;
	RngInstance rngDrawn;
	rngDrawn.isInitialise = FALSE;

	if( initialStatus->delay.distribution != DIST_DETERMINISTIC && initialStatus->delay.seed != -1){
		RandomInitialise(&rngDrawn, initialStatus->delay.seed, initialStatus->delay.seed);
	}

	if(initialStatus->countProbabilisticBranch > 0 && initialStatus->modelSeed > -1){
		RandomInitialise(&rngProbabilisticBranch, initialStatus->modelSeed, initialStatus->modelSeed);
	}

	nReport.idNode = initialStatus->idNode;
	nReport.counterInput = 0 ;
	nReport.amountDelay = 0.0;
	nReport.maximunDrawn = NAN;
	nReport.minimunDrawn = NAN;

	
	int msg = 0;
	do {
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				advancePhaseNormal( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, FALSE, &rngProbabilisticBranch);
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				break;
			case ADVANCE_PAHSE_PRIMA:
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				advancePhaseNormal( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, TRUE, &rngProbabilisticBranch);
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				break;
			case GENERATION_PHASE:
				generationPhaseNormalPrima(&inputWorktask, &outputWorktask, &bodyResource ,commNodes, workTaskList,  initialStatus, FALSE, &nReport, &rngDrawn);
			break;
			case GENERATION_PHASE_PRIMA:
				generationPhaseNormalPrima(&inputWorktask, &outputWorktask, &bodyResource ,commNodes, workTaskList,  initialStatus, TRUE, &nReport, &rngDrawn);
				break;
			case CONSUME_DT:
				deltaTCount++;
				int goOut = discountDelayAndDeleteFinishedWorktask(workTaskList);
				bodyResource -= goOut;
				outputWorktask += goOut;
				MPI_Barrier( commNodes );
				break;
			case PING_REPORT:
			//printf("print report----%d\n",initialStatus->idNode);
			nReport.activityInside = bodyResource;
			
			MPI_Send(&nReport, sizeof(PrinterActivity), MPI_BYTE, PRINTER_ID, NORMAL_REPORT , MPI_COMM_WORLD);
			if(bodyResource > 0){
				double* worktask = delayOfWorktask(workTaskList, bodyResource);
				MPI_Send(worktask, nReport.activityInside* 2, MPI_DOUBLE, PRINTER_ID, NORMAL_REPORT , MPI_COMM_WORLD);
				free(worktask);
			}
			
			default:
				break;
		}
	
	} while (msg != LIVE_LOCK);
	return;
}

void advancePhaseNormal(int * inputWorktask, int* outputWorktask, const Normal *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, RngInstance* rngProbabilisticBranch){ 
	double* walls = NULL;
	int* hollows = NULL;
	int coins = (*outputWorktask);
	if (initialStatus->countProbabilisticBranch > 0){
		walls = (double*) malloc(initialStatus->countProbabilisticBranch * sizeof(double));
		hollows = (int*) malloc(initialStatus->countProbabilisticBranch * sizeof(int)) ;
		walls[ initialStatus->countProbabilisticBranch -1] = 1.0;
		hollows[initialStatus->countProbabilisticBranch -1] = 0;
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
	
	int* bufferReceiver = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
    int receiverCount = 1;
    int msg;
    //inicializo los request de las llegadas de recursos
    MPI_Request* requestPreceders = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countPreceders);
	MPI_Request* requestFollowers = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countFollowers);
    
	//if( isPrima) printf("3: reciviendo los inputs\n");
	//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		 MPI_Irecv( &bufferReceiver[i], receiverCount, MPI_INT,  initialStatus->preceders[i], RESOURCE_SEND, commNodes, &requestPreceders[i]);
	}
	//if( isPrima)printf("3: enciando los outputs\n");
	
	if (initialStatus->countProbabilisticBranch == 0){
		for (int i = 0 ; i < initialStatus->countFollowers; i++){
			 MPI_Isend( outputWorktask, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
		}
	} else {
		for (int i = 0 ; i < initialStatus->countFollowers; i++){
			 MPI_Isend( &hollows[i], 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
		}
	}
	
    //if( isPrima)printf("3: Esperando...1\n");
	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		MPI_Wait(&requestFollowers[i], MPI_STATUS_IGNORE);
		(*outputWorktask) = 0;
	}
	//if( isPrima)printf("3: Esperando...2\n");
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		(*inputWorktask) += bufferReceiver[i];
	}
	
	if( !isPrima ){
		MPI_Barrier( commNodes );
		
	} else {
		int * nodesStatus = NULL;
		msg = (*inputWorktask)? FALSE: TRUE;
		//printf("me quede en la barrera3\n");
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
		//printf(" sale de la barrera3\n");
	}
	
	free(bufferReceiver);
	free(requestPreceders);
	free(requestFollowers);
	return;
}

void generationPhaseNormalPrima(int* inputWorktask, int* outputWorktask, int* bodyResource ,const MPI_Comm commNodes, Worktask *workTaskList,  const Normal *initialStatus, const int isPrima,  PrinterActivity* nReport, RngInstance* rngDrawn){
	//si es deterministico 0
	double humanDelay = 0.0;
	nReport-> counterInput += (*inputWorktask);
	switch(initialStatus->delay.distribution){
		case DIST_DETERMINISTIC:
			for(int i = 0; i < (*inputWorktask); i++){
				humanDelay = initialStatus->delay.constant;
				setNewWorktask(workTaskList, humanDelay, nReport);
			}
		break;

		case DIST_UNIFORM:
        	for(int i = 0; i < (*inputWorktask); i++){
        		humanDelay = RandomDouble(rngDrawn,initialStatus->delay.least, initialStatus->delay.highest);
        		setNewWorktask(workTaskList, humanDelay, nReport);
			}
		break;

		case DIST_NORMAL:
		
    		for(int i = 0; i < (*inputWorktask); i++){
    			humanDelay = RandomNormal(rngDrawn,initialStatus->delay.mean, initialStatus->delay.variance );
    			setNewWorktask(workTaskList, humanDelay, nReport);
			}
		break;

		case DIST_EXPONENTIAL:
			
     		for(int i = 0; i < (*inputWorktask); i++){
     			humanDelay = RandomExponential(rngDrawn, initialStatus->delay.lambda );
     			setNewWorktask(workTaskList, humanDelay, nReport);
			}
		break;

		case DIST_TRIANGULAR:
			
			for(int i = 0; i < (*inputWorktask); i++){
				humanDelay = RandomTriangular(rngDrawn,initialStatus->delay.least, initialStatus->delay.highest, initialStatus->delay.mode);
				setNewWorktask(workTaskList, humanDelay, nReport);
			}
		break;

		case DIST_LOG_NORMAL:
			
			for(int i = 0; i < (*inputWorktask); i++){
				humanDelay =  RandomLogNormalWithMinimun(rngDrawn, initialStatus->delay.escale, initialStatus->delay.shape, initialStatus->delay.least );
				setNewWorktask(workTaskList, humanDelay, nReport);
			}
		break;

		case DIST_BETA:

			for(int i = 0; i < (*inputWorktask); i++){
				humanDelay = RandomBeta(rngDrawn, initialStatus->delay.shapeAlpha, initialStatus->delay.shapeBeta, initialStatus->delay.minimun, initialStatus->delay.maximun );
				setNewWorktask(workTaskList, humanDelay, nReport);
			}
		break;

	}
	
	(*bodyResource) += (*inputWorktask);
	(*inputWorktask) = 0;
	
	int detZero = deleteFinishedWorktask(workTaskList);
	(*bodyResource) -= detZero;
	(*outputWorktask) += detZero;
	
	MPI_Barrier( commNodes );
	//printf("Salgo barrera en barrera");
}
