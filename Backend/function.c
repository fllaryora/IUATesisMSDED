#include "function.h"
#include "genericNode.h"
#include "RNGs.h"

void functionNode( const MPI_Comm commNodes,  const  Function *initialStatus, const int mpiProcesses, const int modelSeed){

	//Un array con el tiempo le falta a cada elemento para terminar (Cantidad de trabajos esta implícito dentro ) (no recursos, porque confundiría)
	//int* cantDeltaTQFalta = NULL;
	//int counterWorkTask = 0;
	//2. Otro array con el tiempo inicial sorteado.
	//int* initialRafflerTime = NULL;
	//int counterRafflerTime = 0;
	//3. Número de recursos que entraron, hasta este delta T.
	//unsigned long long int input = 0;

	int inputResource = 0;//que estan en la entrada antes del cuerpo
	int outputResource = 0; //que cumplieron el dalay se se pueden ir
	int bodyResource = 0; //recources in function
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo
	//On the fly: Promedio de las duraciones sorteadas.
	//TODO:Sumatoria de delay / cant worktask    (la primera vez)
	//(TODO:Sumatoria de delay anterior + Sumatoria de delayde este delta T) / (cant worktask anteriores + cant worktask de este delta T)

	// Máxima duración sorteada
	//double maximun = -1; //maximo de recursos
	// Minima duración sorteada
 	//double minimun = -1; //minimo de recursos

	int msg = 0;
	do {
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputResource,outputResource);
				advancePhaseFunction( &inputResource,  &outputResource, initialStatus, commNodes, mpiProcesses, FALSE, modelSeed);
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputResource,outputResource);
				break;
			case ADVANCE_PAHSE_PRIMA:
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputResource,outputResource);
				advancePhaseFunction( &inputResource,  &outputResource, initialStatus, commNodes, mpiProcesses, TRUE, modelSeed);
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputResource,outputResource);
			break;
			case GENERATION_PHASE: //hace lo mismo que la de abajo
			case GENERATION_PHASE_PRIMA:
				generationPhaseFunction(&inputResource, &bodyResource, &outputResource, commNodes , initialStatus );
				break;
			case CONSUME_DT:
				deltaTCount++;
				MPI_Barrier( commNodes );
				break;
			case PING_REPORT:
			default:
				break;
		}
	
	} while (msg != LIVE_LOCK);
	return;
}

void advancePhaseFunction(int * inputResource, int* outputResource, const Function *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, const int modelSeed){ 
	double* walls = NULL;
	int* hollows = NULL;
	if (initialStatus->countProbabilisticBranch > 0){
		walls = (double*) malloc(initialStatus->countProbabilisticBranch * sizeof(double));
		hollows = (int*) malloc(initialStatus->countProbabilisticBranch * sizeof(int)) ;
	}
	double acummulatedProb = 0.0;
	for(int i = 0; i < initialStatus->countProbabilisticBranch; i++){
		acummulatedProb += initialStatus->probabilisticBranch[i];
		walls[i] = acummulatedProb;
		hollows[i] = 0; //inicializo de paso
	}
	//preeveo errores de redondeo
	if(initialStatus->countProbabilisticBranch > 0){
		walls[ initialStatus->countProbabilisticBranch -1] = 1.0; 
		if(modelSeed != -1 )
			RandomInitialise(modelSeed,modelSeed);
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
	
	
	int* bufferReceiver = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
    int receiverCount = 1;
    int msg;
    //inicializo los request de las llegadas de recursos
    MPI_Request* requestPreceders = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countPreceders);
	MPI_Request* requestFollowers = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countFollowers);
    
	if(isPrima)printf("4: reciviendo los inputs\n");
	//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		 MPI_Irecv( &bufferReceiver[i], receiverCount, MPI_INT,  initialStatus->preceders[i], RESOURCE_SEND, commNodes, &requestPreceders[i]);
	}
	if(isPrima)printf("4: enviando los outputs\n");
	if (initialStatus->countProbabilisticBranch == 0){
		for (int i = 0 ; i < initialStatus->countFollowers; i++){
			 MPI_Isend( outputResource, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
		}
	} else {
		for (int i = 0 ; i < initialStatus->countFollowers; i++){
			 MPI_Isend( &hollows[i], 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
		}
	}
	
    if(isPrima)printf("4: espera en 1...\n");
	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		(*inputResource) += bufferReceiver[i];
	}
	if(isPrima)printf("4: espera en 2...\n");
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		MPI_Wait(&requestFollowers[i], MPI_STATUS_IGNORE);
		(*outputResource) = 0;
	}
	
	
	
	if( !isPrima ){
		
		MPI_Barrier( commNodes );
		
	} else {
		int * nodesStatus = NULL;
		msg = (*inputResource)? FALSE: TRUE;
		printf("me quede en la barrera4\n");
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
		printf(" sale de la barrera4\n");
	}
	free(bufferReceiver);
	free(requestPreceders);
	free(requestFollowers);
	return;
}

void generationPhaseFunction(int* inputResource , int* bodyResource, int* outputResource,  const MPI_Comm commNodes , const Function *initialStatus ){

	(*bodyResource) += (*inputResource);
	(*inputResource) = 0;
	
	int nexOutput = (*bodyResource)/ (initialStatus->input);
	(*bodyResource) %= (initialStatus->input);
	
	(*outputResource) += nexOutput * (initialStatus->output);
	
	//insertWorktask(workTaskList, unsigned long long int currentDelay,  unsigned long long int  initialDelay);
	//printf("espero en barrera generacion contador");
	MPI_Barrier( commNodes );
	//printf("Salgo barrera en barrera");
}
