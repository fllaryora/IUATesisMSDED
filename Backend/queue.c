#include "queue.h"
#include "genericNode.h"


void queueNode( const MPI_Comm commNodes,  const  Queue *initialStatus, const int mpiProcesses){
	int inputResource = 0; //recursos que estan en la entrada 
	int bodyResource = initialStatus->resource; //nivel: cantidad de recursos disponibles en el delta T
		
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo
	
	int msg = 0;
	PrinterQueue qReport;
	qReport.idNode = initialStatus->idNode;
	qReport.maximun = initialStatus->resource;
	qReport.minimun = initialStatus->resource;
	qReport.counterInput = 0;
	int oldCounterInput = 0;
	double oldAverageDelay = 0.0;
	qReport.counterOutput = 0;
	qReport.average = 0.0;
	qReport.timesNotEmpty = 0.0;
	int isTimeNotEmpty;
	qReport.percentTimesNotEmpty = 0.0;
	qReport.averageDelay = 0.0;
	PrinterFinalQueue qReportFinal;
	qReportFinal.idNode = initialStatus->idNode;
	qReportFinal.fixCost = initialStatus->fixedCost;
	
	do {
		
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				advancePhaseQueue(&inputResource, &bodyResource, commNodes, FALSE, initialStatus, mpiProcesses, &qReport );
				break;
			case ADVANCE_PAHSE_PRIMA:
				advancePhaseQueue(&inputResource, &bodyResource, commNodes, TRUE, initialStatus,mpiProcesses, &qReport );
				break;
			case GENERATION_PHASE: //hace lo mismo que la de abajo
				isTimeNotEmpty = FALSE; //de esa forma se hace ell conteo por cada delta T
			case GENERATION_PHASE_PRIMA:
				generationPhaseQueue(&inputResource, &bodyResource, commNodes, &isTimeNotEmpty, &qReport);
				break;
			case CONSUME_DT:
				deltaTCount++;
				qReport.amount = bodyResource;
				int stepsDones = 1;//TODO fixear para paso variable

				qReport.average = (qReport.average * (double)(deltaTCount - stepsDones) + (double)qReport.amount) / (double)deltaTCount;
				if( qReport.minimun > bodyResource  ){
					qReport.minimun = bodyResource;
				}
				if( qReport.maximun < bodyResource ){
					qReport.maximun = bodyResource;
				}
				if(isTimeNotEmpty){
					qReport.timesNotEmpty++;
				}
				qReport.percentTimesNotEmpty = qReport.timesNotEmpty / (double)deltaTCount;
				
				//del calculo tiempo rpomedio de espera
				if( qReport.counterInput != 0 ){
					qReport.averageDelay = ( (double)oldCounterInput * oldAverageDelay + (double)qReport.amount )/((double)qReport.counterInput);
				}

				oldCounterInput = qReport.counterInput;
				oldAverageDelay = qReport.averageDelay;
				MPI_Barrier( commNodes );
				break;
			case PING_REPORT:
			MPI_Send(&qReport, sizeof(PrinterQueue), MPI_BYTE, PRINTER_ID, QUEUE_REPORT , MPI_COMM_WORLD);
			default:
				break;
		}
	} while (msg != LIVE_LOCK);
	qReportFinal.VariableCost = initialStatus->variableCost * qReport.counterOutput;
	MPI_Send(&qReportFinal, sizeof(PrinterFinalQueue), MPI_BYTE, PRINTER_ID, QUEUE_FINAL_REPORT , MPI_COMM_WORLD);
	return;
}

void advancePhaseQueue(int* inputResource, int * bodyResource, const MPI_Comm commNodes, int isPrima, const Queue *initialStatus, const int mpiProcesses, PrinterQueue* qReport ){ 
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
	int* currentFollowerListStatus = (int*) malloc( sizeof(int)* initialStatus->countFollowers ) ;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		currentFollowerListStatus[i] = NOT_PROCESSESED;
	}
	
	while( hasAvailableCombis( currentFollowerListStatus, initialStatus) ){
		
		requestResponceCombis(currentFollowerListStatus, initialStatus, commNodes, (const int *) bodyResource );
		
		getDemandCombis(currentFollowerListStatus, initialStatus, commNodes );
		
		getFortunatedCombis( currentFollowerListStatus, initialStatus, commNodes, bodyResource, qReport );
	}
	
	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		(*inputResource) += bufferReceiver[i];
	}
	
	if( !isPrima ){	
		MPI_Barrier( commNodes );
	} else {
		int * nodesStatus = NULL;
		msg = (*inputResource)? FALSE: TRUE;
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
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
void requestResponceCombis(const int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, const int* bodyResource ){
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
void getDemandCombis(int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes ){
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

void getFortunatedCombis(int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, int* bodyResource,  PrinterQueue* qReport ){
	//Lista de combis afortunadas
	int* prioritaryCombis = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
	int msg;
	int currentStatus = 0;
	int currentIndexStts = 0;
	int currentTag;
	MPI_Status infoComm;
	//tengo mas combis que recursos?
	if( (*bodyResource) < getAvailableCombisNumber( currentFollowerListStatus, initialStatus) ){
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

			if(*bodyResource){
				
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[currentIndexStts] , TRANSACTION_BEGIN, commNodes);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], MPI_ANY_TAG, commNodes, &infoComm);
				currentTag = infoComm.MPI_TAG;
				if(currentTag == TRANSACTION_COMMIT){
					qReport->counterOutput++;
					--(*bodyResource);
				} else {
					currentFollowerListStatus[currentIndexStts] = PROCESSESED;
				}
			} else {
				
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[currentIndexStts] , TRANSACTION_CANCELLED, commNodes);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], TRANSACTION_ROLLBACK, commNodes, MPI_STATUS_IGNORE);
				currentFollowerListStatus[currentIndexStts] = PROCESSESED;
			}
		}
	} else {
		//tengo mas o igual recursos que combis
		//envio a los no procesados el recurso
		for(int i = 0 ; i < initialStatus->countFollowers; i++){
			
			if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[i], TRANSACTION_BEGIN, commNodes);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[i], MPI_ANY_TAG, commNodes, &infoComm);
				currentTag = infoComm.MPI_TAG;
				if(currentTag == TRANSACTION_COMMIT){
					qReport->counterOutput++;
					--(*bodyResource);
					
				} else {
					currentFollowerListStatus[i] = PROCESSESED;
				}
			}
		}
	}
	
	free(prioritaryCombis);
	return;
}

void generationPhaseQueue(int* inputResource, int* bodyResource, const MPI_Comm commNodes, int *isTimeNotEmpty, PrinterQueue* qReport){
	if( (*inputResource) > 0){
		(*isTimeNotEmpty) = TRUE;
		qReport->counterInput += (*inputResource);
	}
	(*bodyResource) += (*inputResource);
	(*inputResource) = 0;
	MPI_Barrier( commNodes );
}
