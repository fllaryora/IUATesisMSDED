#include "combi.h"
#include "genericNode.h"
#include "RNGs.h"

void combiNode( const MPI_Comm commNodes,  const  Combi *initialStatus, const int mpiProcesses, const int modelSeed){

	//Un array con el tiempo le falta a cada elemento para terminar (Cantidad de trabajos esta implícito dentro ) (no recursos, porque confundiría)
	//int* cantDeltaTQFalta = NULL;
	//int counterWorkTask = 0;
	//2. Otro array con el tiempo inicial sorteado.
	//int* initialRafflerTime = NULL;
	//int counterRafflerTime = 0;
	//3. Número de recursos que entraron, hasta este delta T.
	//unsigned long long int input = 0;

	int inputWorktask = 0;//que estan en la entrada antes del cuerpo
	int outputWorktask = 0; //que cumplieron el dalay se se pueden ir
	int bodyResource = 0; //counterWorkTask
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo
	Worktask *workTaskList;
    workTaskList = (Worktask *)malloc(sizeof(Worktask)); //nodo dummy de workstasks
    workTaskList -> next = NULL;

	//On the fly: Promedio de las duraciones sorteadas.
	//TODO:Sumatoria de delay / cant worktask    (la primera vez)
	//(TODO:Sumatoria de delay anterior + Sumatoria de delayde este delta T) / (cant worktask anteriores + cant worktask de este delta T)

	// Máxima duración sorteada
	//double maximun = -1; //maximo de recursos
	// Minima duración sorteada
 	//double minimun = -1; //minimo de recursos

	int msg = 0;
	
	if(initialStatus->countProbabilisticBranch > 0){
		//TODO arreglar el RNG
		//if(modelSeed > -1 )
		//	RandomInitialise(modelSeed,modelSeed);
		//para el rng2
		//if(initialStatus->delay.seed > -1 )
		//		RandomInitialise( initialStatus->delay.seed, initialStatus->delay.seed);
		int seed1, seed2;		
		seed1 = (modelSeed != -1)? modelSeed: initialStatus->delay.seed;
		seed2 = ( initialStatus->delay.seed != -1)? initialStatus->delay.seed:modelSeed;

		if(modelSeed != -1 || initialStatus->delay.seed != -1)
			RandomInitialise(seed1,seed2);
	}
	
	do {
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				advancePhaseCombi( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, FALSE, modelSeed);
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				break;
			case ADVANCE_PAHSE_PRIMA:
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				advancePhaseCombi( &inputWorktask,  &outputWorktask, initialStatus, commNodes, mpiProcesses, TRUE, modelSeed);
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outputWorktask);
				break;
			case GENERATION_PHASE: //hace lo mismo que la de abajo
			case GENERATION_PHASE_PRIMA:
				generationPhaseCombi( &inputWorktask, &bodyResource,  commNodes, workTaskList, initialStatus);
				break;
			case CONSUME_DT:
				deltaTCount++;
				int goOut = discountDelayAndDeleteFinishedWorktask(workTaskList);
				bodyResource -= goOut;
				outputWorktask += goOut;
				MPI_Barrier( commNodes );
				break;
			case PING_REPORT:
			default:
				break;
		}
	
	} while (msg != LIVE_LOCK);
	return;
}


void advancePhaseCombi(int * inputWorktask, int* outputWorktask, const Combi *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, const int modelSeed){ 
	//printf("%d: avance combi\n", initialStatus->idNode);
    for(;;){
    	if(!hasQueueResources( initialStatus, commNodes)){
			if(isPrima)printf("%d: cola sin recursos\n", initialStatus->idNode);
	    	resourcesNoDemand( initialStatus, commNodes);
	    	if(isPrima)printf("%d: no demand\n", initialStatus->idNode);
	    	resourcesSend( initialStatus, commNodes, outputWorktask, modelSeed);
	    	if(isPrima)printf("%d: resource send\n", initialStatus->idNode);
	    	finishCombi( isPrima, commNodes , inputWorktask, mpiProcesses);
	    	if(isPrima)printf("%d: cola sin recursos\n", initialStatus->idNode);
	    	break;
    	}else{
			if(isPrima)printf("%d: cola con recursos\n", initialStatus->idNode);
	    	resourcesDemand(initialStatus, commNodes);
	    	//printf("%d: DEmand\n", initialStatus->idNode);
	    	if(allTransactionBegin( commNodes ,initialStatus)){
				if(isPrima)printf("%d: transaction b\n", initialStatus->idNode);
	    		setAllCommit(initialStatus, commNodes);
	    		if(isPrima)printf("%d: commit\n", initialStatus->idNode);
	    		(*inputWorktask)++;
	    	} else {
				if(isPrima)printf("%d: Else b\n", initialStatus->idNode);
	    		setAllRollback( initialStatus, commNodes);
	    		if(isPrima)printf("%d: rollback b\n", initialStatus->idNode);
	    		resourcesSend( initialStatus, commNodes, outputWorktask, modelSeed);
				if(isPrima)printf("%d: resource send\n", initialStatus->idNode);
	    		finishCombi( isPrima,  commNodes , inputWorktask, mpiProcesses);
	    		if(isPrima)printf("%d: fishish 2 b\n", initialStatus->idNode);
	    		break;
	    	}
    	}
    	if(isPrima)printf("----%d",initialStatus->idNode);
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
		//printf("%d: msg %d\n",initialStatus->idNode,msg);
		allHas &= (msg)?TRUE:FALSE;
		//printf("%d: allHas %d\n",initialStatus->idNode,allHas);
	}
	return allHas;
}

//Envio resource no demand a las colas
void resourcesNoDemand( const Combi *initialStatus, const MPI_Comm commNodes){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_NO_DEMAND, commNodes);
	}
	return ;
}

//Envio resource no demand a las colas
void resourcesDemand( const Combi *initialStatus, const MPI_Comm commNodes){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_DEMAND, commNodes);
	}
	return ;
}

//envio resource send a los followers
//TODO FALTA LA DETERMINISTIC BRANCH
void resourcesSend( const Combi *initialStatus, const MPI_Comm commNodes, int* worktaskInOutput, const int modelSeed){
	
	double* walls = NULL;
	int* hollows = NULL;
	printf("brob branch %d \n",initialStatus->countProbabilisticBranch);
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
		printf("wall i %d= %g \n",i, acummulatedProb);
		hollows[i] = 0; //inicializo de paso
	}
	
	//para cada nodo sortear	
	for(int i = 0; i < initialStatus->countProbabilisticBranch; i++){
		double hollowNumber = RandomUniform();
		//defino donde cae la moneda
		for(int j = 0; j < initialStatus->countProbabilisticBranch; j++){
			if( hollowNumber <= walls[j] ){
				hollows[j]++;
				break;
			}
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


void finishCombi(const int isPrima, const MPI_Comm commNodes ,const int* inputResource, const int mpiProcesses){
	int msg;
	 if( !isPrima ){
		MPI_Barrier( commNodes );
	} else {
		int * nodesStatus = NULL;
		msg = (*inputResource)? FALSE: TRUE;
		printf("me quede en la barrera2\n");
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
		printf(" sale de la barrera2\n");
	}

}

int allTransactionBegin(const MPI_Comm commNodes ,const Combi *initialStatus){
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

void setAllRollback(const Combi *initialStatus, const MPI_Comm commNodes){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], TRANSACTION_ROLLBACK, commNodes);
	}
	return ;
}

void setAllCommit(const Combi *initialStatus, const MPI_Comm commNodes){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], TRANSACTION_COMMIT, commNodes);
	}
	return ;
}

void generationPhaseCombi(int* inputWorktask, int* bodyResource, const MPI_Comm commNodes, Worktask *workTaskList,  const Combi *initialStatus){

	switch(initialStatus->delay.distribution){

		case DIST_DETERMINISTIC:
			for(int i = 0; i < (*inputWorktask); i++){
				//printf("cte = %d", ((int)initialStatus->delay.constant) * TIME_TO_DELTA_T);
				insertWorktask(workTaskList, (int)(initialStatus->delay.constant * TIME_TO_DELTA_T) );
			}
		break;

		case DIST_UNIFORM:
        	for(int i = 0; i < (*inputWorktask); i++){
				insertWorktask(workTaskList, (int)( RandomDouble(initialStatus->delay.least, initialStatus->delay.highest) * TIME_TO_DELTA_T ));
			}
		break;

		case DIST_NORMAL:
		
    		for(int i = 0; i < (*inputWorktask); i++){
				insertWorktask(workTaskList, (int)( RandomNormal(initialStatus->delay.mean, initialStatus->delay.variance )  * TIME_TO_DELTA_T ));
			}
		break;

		case DIST_EXPONENTIAL:
			
     		for(int i = 0; i < (*inputWorktask); i++){
				insertWorktask(workTaskList, (int)( RandomExponential( initialStatus->delay.lambda )  * TIME_TO_DELTA_T ));
			}
		break;

		case DIST_TRIANGULAR:
			
			for(int i = 0; i < (*inputWorktask); i++){
				insertWorktask(workTaskList, (int)( RandomTriangular(initialStatus->delay.least, initialStatus->delay.highest, initialStatus->delay.mode) * TIME_TO_DELTA_T ));
			}
		break;

		case DIST_LOG_NORMAL:
			
			for(int i = 0; i < (*inputWorktask); i++){
				insertWorktask(workTaskList, (int)( RandomLogNormalWithMinimun( initialStatus->delay.escale, initialStatus->delay.shape, initialStatus->delay.least )  * TIME_TO_DELTA_T ));
			}
		break;

		case DIST_BETA:
			//TODO : falta completar el caso de que solo uno de los dos tiene parametro mayor a uno
			if(initialStatus->delay.shapeAlpha < 1 && initialStatus->delay.shapeBeta < 1){
				for(int i = 0; i < (*inputWorktask); i++){
					insertWorktask(workTaskList, (int)( RandomBetaWithMinimunAndMaximun( initialStatus->delay.shapeAlpha, initialStatus->delay.shapeBeta, initialStatus->delay.minimun, initialStatus->delay.maximun )  * TIME_TO_DELTA_T ));
				}
			} else {
				for(int i = 0; i < (*inputWorktask); i++){
					insertWorktask(workTaskList, (int)( RandomBetaIntegerWithMinimunAndMaximun( (int)initialStatus->delay.shapeAlpha, (int)initialStatus->delay.shapeBeta, initialStatus->delay.minimun, initialStatus->delay.maximun )  * TIME_TO_DELTA_T ));
				}	
			}
			
		break;

	}
		
	(*bodyResource) += (*inputWorktask);
	(*inputWorktask) = 0;

	int detZero = deleteFinishedWorktask(workTaskList);
	(*bodyResource) -= detZero;
	(*outputWorktask) += detZero;

	//printf("espero en barrera");
	MPI_Barrier( commNodes );
}
