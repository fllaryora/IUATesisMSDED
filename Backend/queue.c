#include "queue.h"
#include "genericNode.h"


void queueNode( const MPI_Comm commNodes,  const  Queue *initialStatus, const int mpiProcesses){
	
	int inputResource = 0; //recursos que estan en la entrada 
	int bodyResource = initialStatus->resource; //nivel: cantidad de recursos disponibles en el delta T
	//unsigned long long int input = 0; //entradas: numero de recursos que entraron en la cola
	//unsigned long long int output = 0; //salidas: numero de recursos que salieron de la cola
	
	//promedio de recursos en espera ON the fly// recursoss en espera acumulados / delta t
	//int maximun = initialStatus->resource; //maximo de recursos
	//int minimun = initialStatus->resource; //minimo de recursos
	
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo
	//unsigned long long int delatedResourcesAccum = 0; //numero de recursos en espera acumulados
	//unsigned long long int counterDeltaTNotEmpty = 0;//contador de delta T no vacios
	//On the fly Porcentaje (contador de delta T no vacios / contador de delta T) *100
	int msg = 0;

	do {
		
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				printf("%d: entrada: %d, cuerpo %d\n", initialStatus->idNode,inputResource,bodyResource);
				advancePhaseQueue(&inputResource, &bodyResource, commNodes, FALSE, initialStatus, mpiProcesses);
				printf("%d: entrada: %d, cuerpo %d\n", initialStatus->idNode,inputResource,bodyResource);
				break;
			case ADVANCE_PAHSE_PRIMA:
				advancePhaseQueue(&inputResource, &bodyResource, commNodes, TRUE, initialStatus,mpiProcesses);
				break;
			case GENERATION_PHASE:
			case GENERATION_PHASE_PRIMA:
				generationPhaseQueue(&inputResource, &bodyResource, commNodes);
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

void advancePhaseQueue(int* inputResource, int * bodyResource, const MPI_Comm commNodes, int isPrima, const Queue *initialStatus, const int mpiProcesses){ 
    int* bufferReceiver = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
    int receiverCount = 1;
    int msg;
    //inicializo los request de las llegadas de recursos
    
    MPI_Request* requestPreceders = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countPreceders);
	//printf("%d: inicio\n", initialStatus->idNode);
	//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		 MPI_Irecv( &bufferReceiver[i], receiverCount, MPI_INT,  initialStatus->preceders[i], RESOURCE_SEND, commNodes, &requestPreceders[i]);
	}
	//printf("%d: Envios pendientes\n", initialStatus->idNode);
	//armo la lista de nodos no procesados
	int* currentFollowerListStatus = (int*) malloc( sizeof(int)* initialStatus->countFollowers ) ;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		currentFollowerListStatus[i] = NOT_PROCESSESED;
	}
	printf("%d: pasa los processed\n", initialStatus->idNode);
	while( hasAvailableCombis( currentFollowerListStatus, initialStatus) ){
		//printf("%d: has aviables\n", initialStatus->idNode);
		requestResponceCombis(currentFollowerListStatus, initialStatus, commNodes, (const int *) bodyResource);
		//printf("%d: request responces\n", initialStatus->idNode);
		getDemandCombis(currentFollowerListStatus, initialStatus, commNodes);
		//printf("%d: fortunated\n", initialStatus->idNode);
		getFortunatedCombis( currentFollowerListStatus, initialStatus, commNodes, bodyResource);
	}
	printf("%d: Envios pendientes\n", initialStatus->idNode);
	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		(*inputResource) += bufferReceiver[i];
	}
	//printf("%d: SALi Envios pendientes\n", initialStatus->idNode);
	if( !isPrima ){
		MPI_Barrier( commNodes );
	} else {
		int * nodesStatus = NULL;
		msg = (*inputResource)? FALSE: TRUE;
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, (mpiProcesses - RAFFLER_PRINTER) , MPI_INT,  MASTER_ID, commNodes);
	}

	free(bufferReceiver);
	free(requestPreceders);
	free(currentFollowerListStatus);
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
void requestResponceCombis(const int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, const int* bodyResource){
	int msg;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
			MPI_Recv( &msg, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_REQUEST, commNodes, MPI_STATUS_IGNORE);
			msg = (*bodyResource);
			MPI_Send( &msg, 1, MPI_INT, initialStatus->followers[i], RESOURCE_RESPONSE, commNodes);
		}
	}
}

//Espero la respuesta si es un resource demand o no demand por parte de la combi
//La combi que tiene una cola sin recurso no demandara
void getDemandCombis(int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes){
	int msg;
	int currentTag;
	MPI_Status infoComm;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
			MPI_Recv( &msg, 1, MPI_INT,  initialStatus->followers[i], MPI_ANY_TAG, commNodes, &infoComm);
			currentTag = infoComm.MPI_TAG;
			if(currentTag == RESOURCE_NO_DEMAND){
				printf("%d: recibi resource no demand\n", initialStatus->idNode);
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

void getFortunatedCombis(int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, int* bodyResource){
	//Lista de combis afortunadas
	int* prioritaryCombis = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
	int msg;
	int currentStatus = 0;
	int currentIndexStts = 0;
	int currentTag;
	MPI_Status infoComm;
	printf("%d: dentro del fortunated\n", initialStatus->idNode);
	if( (*bodyResource) < getAvailableCombisNumber( currentFollowerListStatus, initialStatus) ){
		printf("%d: tengo mas combis que recursos\n", initialStatus->idNode);
		//obtengo la lista de prioridades
		MPI_Send(initialStatus->followers, initialStatus->countFollowers, MPI_INT, RAFFLER_ID, GET_RAFFLE, MPI_COMM_WORLD);
		printf("%d: Envio listade prioridades\n", initialStatus->idNode);
		MPI_Recv( prioritaryCombis, initialStatus->countFollowers, MPI_INT, RAFFLER_ID, RAFFLE_DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("%d: recibo listade prioridades\n", initialStatus->idNode);
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

			if(*bodyResource){
				printf("%d: Envio recurso, a %d\n", initialStatus->idNode, initialStatus->followers[currentIndexStts]);
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[currentIndexStts] , TRANSACTION_BEGIN, commNodes);
				printf("%d: Envio recurso\n", initialStatus->idNode);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], MPI_ANY_TAG, commNodes, &infoComm);
				currentTag = infoComm.MPI_TAG;
				if(currentTag == TRANSACTION_COMMIT){
					printf("%d: recibo el ok\n", initialStatus->idNode);
					--(*bodyResource);
				} else {
					printf("%d: recibo el no ok\n", initialStatus->idNode);
					currentFollowerListStatus[currentIndexStts] = PROCESSESED;
				}
			} else {
				printf("%d: Envio la cancelada\n", initialStatus->idNode);
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[currentIndexStts] , TRANSACTION_CANCELLED, commNodes);
				printf("%d: Envio la cancelada\n", initialStatus->idNode);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], TRANSACTION_ROLLBACK, commNodes, MPI_STATUS_IGNORE);
				printf("%d: recibo el ok de la cancelada\n", initialStatus->idNode);
				currentFollowerListStatus[currentIndexStts] = PROCESSESED;
			}
		}
	} else {
		printf("%d: tengo mas o igial recursos que combis\n", initialStatus->idNode);
		//envio a los no procesados el recurso
		for(int i = 0 ; i < initialStatus->countFollowers; i++){
			
			if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
				printf("%d: Envio recurso, a %d\n", initialStatus->idNode, initialStatus->followers[i]);
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[i], TRANSACTION_BEGIN, commNodes);
				printf("%d: Envio recurso\n", initialStatus->idNode);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[i], MPI_ANY_TAG, commNodes, &infoComm);
				currentTag = infoComm.MPI_TAG;
				if(currentTag == TRANSACTION_COMMIT){
					printf("%d: recibo el ok\n", initialStatus->idNode);
					--(*bodyResource);
				} else {
					printf("%d: recibo el no ok\n", initialStatus->idNode);
					currentFollowerListStatus[i] = PROCESSESED;
				}
			}
		}
	}
	
	free(prioritaryCombis);
	return;
}

void generationPhaseQueue(int* inputResource, int* bodyResource, const MPI_Comm commNodes){
		(*bodyResource) += (*inputResource);
		(*inputResource) = 0;
		MPI_Barrier( commNodes );
}
