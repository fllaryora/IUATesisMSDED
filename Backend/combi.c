#include "queue.h"
#include "genericNode.h"


void combiNode( const MPI_Comm commNodes,  const  Combi *initialStatus, const int mpiProcesses){

	//Un array con el tiempo le falta a cada elemento para terminar (Cantidad de trabajos esta implícito dentro ) (no recursos, porque confundiría)
	int* cantDeltaTQFalta = NULL;
	int counterWorkTask = 0;
	//2. Otro array con el tiempo inicial sorteado.
	int* initialRafflerTime = NULL;
	int counterRafflerTime = 0;
	//3. Número de recursos que entraron, hasta este delta T.
	unsigned long long int input = 0;

	int inputWorktask = 0;//que estan en la entrada antes del cuerpo
	int outPutWorktask = 0; //que cumplieron el dalay se se pueden ir
	
	//On the fly: Promedio de las duraciones sorteadas.
	//TODO:Sumatoria de delay / cant worktask    (la primera vez)
	//(TODO:Sumatoria de delay anterior + Sumatoria de delayde este delta T) / (cant worktask anteriores + cant worktask de este delta T)

	// Máxima duración sorteada
	double maximun = -1; //maximo de recursos
	// Minima duración sorteada
 	double minimun = -1; //minimo de recursos

	int msg = 0;
	do {
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				advancePhaseCombi( inputWorktask,  outPutWorktask, initialStatus, commNodes, mpiProcesses, FALSE);
				break;
			case ADVANCE_PAHSE_PRIMA:

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


void advancePhaseCombi(int & inputWorktask, int & outPutWorktask, const Counter *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima){ 
    int* bufferReceiver = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
    int receiverCount = 1;
    int msg;

    for(;;){
    	if(!hasQueueResources( initialStatus, commNodes)){
	    	resourcesNoDemand( initialStatus, commNodes);
	    	resourcesSend( initialStatus, commNodes, outPutWorktask);
	    	finishCombi( isPrima, commNodes , inputWorktask, mpiProcesses);
	    	break;
    	}else{
	    	resourcesDemand(initialStatus, commNodes);
	    	if(allTransactionBegin( commNodes ,initialStatus)){
	    		setAllCommit(initialStatus, commNodes);
	    		inputWorktask++;
	    	} else {
	    		setAllRollback( initialStatus, commNodes);
	    		finishCombi( isPrima,  commNodes , inputWorktask, mpiProcesses);
	    		break;
	    	}
    	}
    }
	return;
}

//Espero a que todas las combis me manden resource request
int hasQueueResources( const Counter *initialStatus, const MPI_Comm commNodes){
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
void resourcesNoDemand( const Counter *initialStatus, const MPI_Comm commNodes){
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_NO_DEMAND, commNodes);
	}
	return ;
}

//Envio resource no demand a las colas
void resourcesDemand( const Counter *initialStatus, const MPI_Comm commNodes){
	int msg;
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], RESOURCE_DEMAND, commNodes);

	}
	return ;
}

//envio resource send a los followers
//TODO FALTA LA DETERMINISTIC BRANCH
void resourcesSend( const Counter *initialStatus, const MPI_Comm commNodes, int worktaskInOutput){
    //inicializo los request de las llegadas de recursos
    MPI_Request* requestPreceders = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countFollowers);
	//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		 MPI_Isend( &worktaskInOutput, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestPreceders[i]);
	}

	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		worktaskInOutput = 0;
	}
}


void finishCombi(const int isPrima, const MPI_Comm commNodes ,const int inputResource, const int mpiProcesses){

	 if( !isPrima ){
		MPI_Barrier( commNodes );
	} else {
		int * nodesStatus = NULL;
		msg = inputResource? FALSE: TRUE;
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, (mpiProcesses - RAFFLER_PRINTER) , MPI_INT,  MASTER_ID, commNodes);
	}

}

int allTransactionBegin(const MPI_Comm commNodes ,const Counter *initialStatus){
	int msg;
	int allBegin = TRUE;
	MPI_Status infoComm;

	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Recv( &msg, 1, MPI_INT, initialStatus->preceders[i], MPI_ANY_TAG, commNodes, &infoComm);
		currentTag = infoComm.MPI_TAG;
		allBegin  &= (currentTag == TRANSACTION_BEGIN)?TRUE:FALSE;
	}
	return allBegin;
}

void setAllRollback(const Counter *initialStatus, const MPI_Comm commNodes){
	int msg;
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], TRANSACTION_ROLLBACK, commNodes);
	}
	return ;
}

void setAllCommit(const Counter *initialStatus, const MPI_Comm commNodes){
	int msg;
	for(int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Send( NULL, 0, MPI_INT,  initialStatus->preceders[i], TRANSACTION_COMMIT, commNodes);
	}
	return ;
}
