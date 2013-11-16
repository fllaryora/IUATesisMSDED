#include "queue.h"
#include "genericNode.h"


void queueNode( const MPI_Comm commNodes,  const  Queue *initialStatus){
	
	int inputResource = 0; //recursos que estan en la entrada 
	int bodyResource = initialStatus->resource; //nivel: cantidad de recursos disponibles en el delta T
	unsigned long long int input = 0; //entradas: numero de recursos que entraron en la cola
	unsigned long long int output = 0; //salidas: numero de recursos que salieron de la cola
	
	//promedio de recursos en espera ON the fly// recursoss en espera acumulados / delta t
	int maximun = initialStatus->resource; //maximo de recursos
	int minimun = initialStatus->resource; //minimo de recursos
	
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo
	unsigned long long int delatedResourcesAccum = 0; //numero de recursos en espera acumulados
	unsigned long long int counterDeltaTNotEmpty = 0;//contador de delta T no vacios
	//On the fly Porcentaje (contador de delta T no vacios / contador de delta T) *100
	int msg = 0;

	do {
		
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				advancePhaseQueue(inputResource, bodyResource, commNodes, FALSE, initialStatus);
				break;
			case ADVANCE_PAHSE_PRIMA:
				advancePhaseQueue(inputResource, bodyResource, commNodes, TRUE, initialStatus);
				break;
			case GENERATION_PHASE:
			case GENERATION_PHASE_PRIMA:
				generationPhaseQueue(inputResource, bodyResource, commNodes);
				break;
			case CONSUME_DT:
				deltaTCount++;
				break;
			case PING_REPORT:
			default:
				break;
		}
	
	} while (msg != LIVE_LOCK);

	return;
}

void advancePhaseQueue(int & inputResource, int & bodyResource, const MPI_Comm commNodes, int isPrima, Queue *initialStatus, const int mpiProcesses){ 
    int* bufferReceiver = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
    int receiverCount = 1;
    int msg;
    //inicializo los request de las llegadas de recursos
    MPI_Request* requestPreceders = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countPreceders);

	//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		 MPI_Irecv( &bufferReceiver[i], receiverCount, MPI_INT,  initialStatus->preceders[i], RESOURCE_SEND, commNodes, &requestPreceders[i]);
	}

	//armo la lista de nodos no procesados
	int* currentFollowerListStatus = (int*) mallc( sizeof(int)* initialStatus->countFollowers ) ;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		currentFollowerListStatus[i] = NOT_PROCESSESED;
	}

	while( hasAvailableCombis( currentFollowerListStatus, initialStatus) ){
		requestResponceCombis(currentFollowerListStatus, initialStatus, commNodes,  bodyResource);
		getDemandCombis(currentFollowerListStatus, initialStatus, commNodes, bodyResource);
		getFortunatedCombis( currentFollowerListStatus, initialStatus, commNodes, bodyResource);
	}

	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		inputResource += bufferReceiver[i];
	}

	if( !isPrima ){
		MPI_Barrier( commNodes );
	} else {
		int * nodesStatus = NULL;
		msg = inputResource? FALSE: TRUE;
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, (mpiProcesses - RAFFLER_PRINTER) , MPI_INT,  MASTER_ID, commNodes);
	}
	return;
}

// ¿tengo caombis no procesadas en la lista?
int hasAvailableCombis(const int* currentFollowerListStatus, const Queue *initialStatus){
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		if( currentFollowerListStatus[i] == NOT_PROCESSESED )
			return TRUE;
	}
	return FALSE;
}

//Espero a que todas las combis me manden resource request
int requestResponceCombis(const int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, const int bodyResource){
	int msg;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
			MPI_Recv( &msg, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_REQUEST, commNodes, MPI_STATUS_IGNORE);
			msg = bodyResource;
			MPI_Send( &msg, 1, MPI_INT, initialStatus->followers[i], RESOURCE_RESPONSE, commNodes);
		}
	}
}

//Espero la respuesta si es un resource demand o no demand por parte de la combi
//La combi que tiene una cola sin recurso no demandara
void getDemandCombis(int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, const int bodyResource){
	int msg;
	int currentTag;
	MPI_Status infoComm;

	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
			MPI_Recv( &msg, 1, MPI_INT,  initialStatus->followers[i], MPI_ANY_TAG, commNodes, &infoComm);
			currentTag = infoComm.MPI_TAG;
			if(currentTag == RESOURCE_NO_DEMAND){
				currentFollowerListStatus[i] = 	PROCESSESED;
			}
		}
	}
}

// ¿tengo caombis no procesadas en la lista?
int getAvailableCombisNumber(const int* currentFollowerListStatus, const Queue *initialStatus){
	int counter = 0;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		if( currentFollowerListStatus[i] == NOT_PROCESSESED )
			counter++;
	}
	return counter;
}

void getFortunatedCombis(int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, int& bodyResource){
	//Lista de combis afortunadas
	int* prioritaryCombis = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
	int msg;
	int currentRec = 0;
	int currentStatus ;
	int currentIndexStts ;
	if( bodyResource < getAvailableCombisNumber( currentFollowerListStatus, initialStatus) ){
		//obtengo la lista de prioridades
		MPI_Send(initialStatus->followers, initialStatus->countFollowers, MPI_INT, RAFFLER_ID, GET_RAFFLE, MPI_COMM_WORLD);
		MPI_Recv( prioritaryCombis, initialStatus->countFollowers, MPI_INT, RAFFLER_ID, RAFFLE_DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//por cada id de combi afortunada no procesadda, enviarle un recurso
		//ya al resto de combis no afortunadas y no pricesadas cancelar y procesarla
		for(int i = 0 ; i < initialStatus->countFollowers; i++){ //prioritaryCombis
			//get current status
			for(int j = 0 ; j < initialStatus->countFollowers; j++){// initialStatus->followers
				if( prioritaryCombis[i] == initialStatus->followers[j] ){
					currentStatus = currentFollowerListStatus[j];
					currentIndexStts = j;
					break;
				}
			}
			//No hago nada
			if(currentStatus == PROCESSESED)
				continue;

			if(bodyResource){
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[currentIndexStts] , TRANSACTION_BEGIN, commNodes);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], TRANSACTION_COMMIT, commNodes, MPI_STATUS_IGNORE);
				--bodyResource;
			} else {
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[currentIndexStts] , TRANSACTION_CANCELLED, commNodes);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], TRANSACTION_ROLLBACK, commNodes, MPI_STATUS_IGNORE);
				currentFollowerListStatus[currentIndexStts] = PROCESSESED;
			}
		}
	} else {
		//envio a los no procesados el recurso
		for(int i = 0 ; i < initialStatus->countFollowers; i++){
			if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[currentIndexStts], TRANSACTION_BEGIN, commNodes);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], TRANSACTION_COMMIT, commNodes, MPI_STATUS_IGNORE);
				--bodyResource;
			}
		}
	}

}

void generationPhaseQueue(int & inputResource, int & bodyResource, const MPI_Comm commNodes){
		bodyResource += inputResource;
		inputResource = 0;
		MPI_Barrier( commNodes );
}
