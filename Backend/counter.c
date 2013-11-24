#include "counter.h"
#include "genericNode.h"


void counterNode( const MPI_Comm commNodes,  const  Counter *initialStatus, const int mpiProcesses){

	//Un array con el tiempo le falta a cada elemento para terminar (Cantidad de trabajos esta implícito dentro ) (no recursos, porque confundiría)
	//int* cantDeltaTQFalta = NULL;
	//int counterWorkTask = 0;
	//2. Otro array con el tiempo inicial sorteado.
	//int* initialRafflerTime = NULL;
	//int counterRafflerTime = 0;
	//3. Número de recursos que entraron, hasta este delta T.
	//unsigned long long int input = 0;

	int inputWorktask = 0;//que estan en la entrada antes del cuerpo
	int outPutWorktask = 0; //que cumplieron el dalay se se pueden ir
	    

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
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outPutWorktask);
				advancePhaseCounter( &inputWorktask,  &outPutWorktask, initialStatus, commNodes, mpiProcesses, FALSE);
				printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputWorktask,outPutWorktask);
				break;
			case ADVANCE_PAHSE_PRIMA:
			advancePhaseCounter( &inputWorktask,  &outPutWorktask, initialStatus, commNodes, mpiProcesses, TRUE);
				break;
			case GENERATION_PHASE:
			case GENERATION_PHASE_PRIMA:
			case CONSUME_DT:
			case PING_REPORT:
			default:
				break;
		}
	
	} while (msg != LIVE_LOCK);
	return;
}

void advancePhaseCounter(int * inputWorktask, int* outPutWorktask, const Counter *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima){ 
	int* bufferReceiver = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
    int receiverCount = 1;
    int msg;
    //inicializo los request de las llegadas de recursos
    MPI_Request* requestPreceders = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countPreceders);
	MPI_Request* requestFollowers = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countFollowers);
    
	printf("reciviendo los inputs\n");
	//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		 MPI_Irecv( &bufferReceiver[i], receiverCount, MPI_INT,  initialStatus->preceders[i], RESOURCE_SEND, commNodes, &requestPreceders[i]);
	}
	printf("enviando los outpus\n");
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		 MPI_Isend( outPutWorktask, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
	}


	printf("Espero followrs\n");
	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		MPI_Wait(&requestFollowers[i], MPI_STATUS_IGNORE);
		(*outPutWorktask) = 0;
	}
	
	printf("Espero predecesor\n");
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		(*inputWorktask) += bufferReceiver[i];
	}
   
	
	
	if( !isPrima ){
		printf("me quede en la barrera5\n");
		MPI_Barrier( commNodes );
		printf(" sale de la barrera\n");
	} else {
		int * nodesStatus = NULL;
		msg = (*inputWorktask)? FALSE: TRUE;
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, (mpiProcesses - RAFFLER_PRINTER) , MPI_INT,  MASTER_ID, commNodes);
	}

	free(bufferReceiver);
	free(requestPreceders);
	free(requestFollowers);
	return;
}
