#include "queue.h"
#include "genericNode.h"


void queueNode( const MPI_Comm commNodes,  const  Queue *initialStatus, const int mpiProcesses){
	char* fileName = NULL;
	int len = snprintf(NULL, 0, "/tmp/queue.%d.log",initialStatus->idNode);
	fileName = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(fileName, (len + 1), "/tmp/queue.%d.log",initialStatus->idNode);
	
	int fileDescriptor = open (fileName, O_WRONLY|O_CREAT|O_TRUNC, 00660);
	int inputResource = 0; //recursos que estan en la entrada 
	int bodyResource = initialStatus->resource; //nivel: cantidad de recursos disponibles en el delta T
		
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo
	
	int msg = 0;
	PrinterQueue qReport;
	qReport.idNode = initialStatus->idNode;
	qReport.maximun = initialStatus->resource;
	qReport.minimun = initialStatus->resource;
	qReport.counterInput = 0;
	qReport.counterOutput = 0;
	qReport.average = 0.0;
	qReport.timesNotEmpty = 0.0;
	int isTimeNotEmpty;
	int amountInput ;
	qReport.percentTimesNotEmpty = 0.0;
	
	
	PrinterFinalQueue qReportFinal;
	qReportFinal.idNode = initialStatus->idNode;
	qReportFinal.fixCost = initialStatus->fixedCost;
	
	loger( fileDescriptor, "-- Inicio --\n");
	do {
		
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				logPhase(fileDescriptor,"Avance Phase input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
				advancePhaseQueue(&inputResource, &bodyResource, commNodes, FALSE, initialStatus, mpiProcesses, &qReport, fileDescriptor);
				logPhase(fileDescriptor,"End Avance Phase input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
				break;
			case ADVANCE_PAHSE_PRIMA:
				logPhase(fileDescriptor,"Avance Phase Prima input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
				advancePhaseQueue(&inputResource, &bodyResource, commNodes, TRUE, initialStatus,mpiProcesses, &qReport, fileDescriptor);
				logPhase(fileDescriptor,"End Avance Phase Prima input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
				break;
			case GENERATION_PHASE: //hace lo mismo que la de abajo
				isTimeNotEmpty = FALSE; //de esa forma se hace ell conteo por cada delta T
				amountInput = 0;
			case GENERATION_PHASE_PRIMA:
				logPhase(fileDescriptor,"Generation Phase input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
				generationPhaseQueue(&inputResource, &bodyResource, commNodes, &isTimeNotEmpty, &amountInput, &qReport);
				logPhase(fileDescriptor,"End generation Phase input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
				break;
			case CONSUME_DT:
				logPhase(fileDescriptor,"Consume DT input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
				deltaTCount++;
				qReport.amount = bodyResource;
				qReport.average = (qReport.average*(double)(deltaTCount-1) + (double)qReport.amount) / (double)deltaTCount;
				if( qReport.minimun > amountInput  ){
					qReport.minimun = amountInput;
				}
				if( qReport.maximun < amountInput ){
					qReport.maximun = amountInput;
				}
				if(isTimeNotEmpty){
					qReport.timesNotEmpty++;
				}
				qReport.percentTimesNotEmpty = qReport.timesNotEmpty / (double)deltaTCount;
				
				MPI_Barrier( commNodes );
				logPhase(fileDescriptor,"End consume DT input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
				break;
			case PING_REPORT:
			logPhase(fileDescriptor,"Ping Report input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
			MPI_Send(&qReport, sizeof(PrinterQueue), MPI_BYTE, PRINTER_ID, QUEUE_REPORT , MPI_COMM_WORLD);
			logPhase(fileDescriptor,"End ping report input = %d, body = %d , output = %d\n",inputResource,bodyResource,bodyResource);
			default:
				break;
		}
	} while (msg != LIVE_LOCK);
	loger( fileDescriptor, "-- LIVE_LOCK --\n");
	qReportFinal.VariableCost = initialStatus->variableCost * qReport.counterOutput;
	loger( fileDescriptor, "-- Sending final report --\n");
	MPI_Send(&qReportFinal, sizeof(PrinterFinalQueue), MPI_BYTE, PRINTER_ID, QUEUE_FINAL_REPORT , MPI_COMM_WORLD);
	loger( fileDescriptor, "--Finish Sending final report --\n");
	close(fileDescriptor);
	return;
}

void advancePhaseQueue(int* inputResource, int * bodyResource, const MPI_Comm commNodes, int isPrima, const Queue *initialStatus, const int mpiProcesses, PrinterQueue* qReport, int fileDescriptor){ 
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
		
		requestResponceCombis(currentFollowerListStatus, initialStatus, commNodes, (const int *) bodyResource,fileDescriptor);
		
		getDemandCombis(currentFollowerListStatus, initialStatus, commNodes,fileDescriptor);
		
		getFortunatedCombis( currentFollowerListStatus, initialStatus, commNodes, bodyResource, qReport,fileDescriptor);
	}
	
	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		logPhase(fileDescriptor,"<<< RESOURCE_SEND = %d <<< N%d=%d \n", bufferReceiver[i] , i,  initialStatus->preceders[i]);
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
void requestResponceCombis(const int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, const int* bodyResource, int fileDescriptor){
	int msg;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
			MPI_Recv( &msg, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_REQUEST, commNodes, MPI_STATUS_IGNORE);
			loger( fileDescriptor, "<<< RESOURCE_REQUEST <<<\n");
			msg = (*bodyResource);
			MPI_Send( &msg, 1, MPI_INT, initialStatus->followers[i], RESOURCE_RESPONSE, commNodes);
			
		}
	}
}

//Espero la respuesta si es un resource demand o no demand por parte de la combi
//La combi que tiene una cola sin recurso no demandara
void getDemandCombis(int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, int fileDescriptor){
	int msg;
	int currentTag;
	MPI_Status infoComm;
	for(int i = 0 ; i < initialStatus->countFollowers; i++){
		if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
			MPI_Recv( &msg, 1, MPI_INT,  initialStatus->followers[i], MPI_ANY_TAG, commNodes, &infoComm);
			currentTag = infoComm.MPI_TAG;
			if(currentTag == RESOURCE_NO_DEMAND){
				logPhase(fileDescriptor,"%d <<< RESOURCE_NO_DEMAND <<< C%d = %d \n",0,i,initialStatus->followers[i]);
				currentFollowerListStatus[i] = 	PROCESSESED;
			} else {
				logPhase(fileDescriptor,"%d <<< RESOURCE_DEMAND <<< C%d = %d \n",0,i,initialStatus->followers[i]);
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

void getFortunatedCombis(int* currentFollowerListStatus, const Queue *initialStatus, const MPI_Comm commNodes, int* bodyResource,  PrinterQueue* qReport, int fileDescriptor){
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
		loger( fileDescriptor, ">>> GET_RAFFLE >>>\n");
		MPI_Recv( prioritaryCombis, initialStatus->countFollowers, MPI_INT, RAFFLER_ID, RAFFLE_DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		loger( fileDescriptor, "<<< RAFFLE_DONE <<< Lista de prioridades\n");
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
				logPhase(fileDescriptor,">>> C%d = %d >>> TRANSACTION_BEGIN >>> %d\n",currentIndexStts,initialStatus->followers[currentIndexStts],0);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], MPI_ANY_TAG, commNodes, &infoComm);
				currentTag = infoComm.MPI_TAG;
				if(currentTag == TRANSACTION_COMMIT){
					logPhase(fileDescriptor,"%d <<< TRANSACTION_COMMIT <<< C%d = %d \n",0,currentIndexStts,initialStatus->followers[currentIndexStts]);
					qReport->counterOutput++;
					--(*bodyResource);
				} else {
					logPhase(fileDescriptor,"%d <<< TRANSACTION_ROLLBACK <<< C%d = %d \n",0,currentIndexStts,initialStatus->followers[currentIndexStts]);
					currentFollowerListStatus[currentIndexStts] = PROCESSESED;
				}
			} else {
				
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[currentIndexStts] , TRANSACTION_CANCELLED, commNodes);
				logPhase(fileDescriptor,">>> C%d = %d >>> TRANSACTION_CANCELLED >>> %d\n",currentIndexStts,initialStatus->followers[currentIndexStts],0);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[currentIndexStts], TRANSACTION_ROLLBACK, commNodes, MPI_STATUS_IGNORE);
				logPhase(fileDescriptor,"%d <<< TRANSACTION_ROLLBACK <<< C%d = %d \n",0,currentIndexStts,initialStatus->followers[currentIndexStts]);
				currentFollowerListStatus[currentIndexStts] = PROCESSESED;
			}
		}
	} else {
		//tengo mas o igual recursos que combis
		//envio a los no procesados el recurso
		for(int i = 0 ; i < initialStatus->countFollowers; i++){
			
			if( currentFollowerListStatus[i] == NOT_PROCESSESED ){
				MPI_Send( NULL, 0, MPI_INT, initialStatus->followers[i], TRANSACTION_BEGIN, commNodes);
				logPhase(fileDescriptor,">>> C%d = %d >>> TRANSACTION_BEGIN >>> %d\n",i,initialStatus->followers[i],0);
				MPI_Recv( &msg, 1, MPI_INT, initialStatus->followers[i], MPI_ANY_TAG, commNodes, &infoComm);
				currentTag = infoComm.MPI_TAG;
				if(currentTag == TRANSACTION_COMMIT){
					logPhase(fileDescriptor,"%d <<< TRANSACTION_COMMIT <<< C%d = %d \n",0,i,initialStatus->followers[i]);
					qReport->counterOutput++;
					--(*bodyResource);
					
				} else {
					logPhase(fileDescriptor,"%d <<< TRANSACTION_ROLLBACK <<< C%d = %d \n",0,i,initialStatus->followers[i]);
					currentFollowerListStatus[i] = PROCESSESED;
				}
			}
		}
	}
	
	free(prioritaryCombis);
	return;
}

void generationPhaseQueue(int* inputResource, int* bodyResource, const MPI_Comm commNodes, int *isTimeNotEmpty, int* amountInput, PrinterQueue* qReport){
	if( (*inputResource) > 0){
		(*isTimeNotEmpty) = TRUE;
		(*amountInput) += (*inputResource);
		qReport->counterInput += (*inputResource);
	}
	(*bodyResource) += (*inputResource);
	(*inputResource) = 0;
	MPI_Barrier( commNodes );
}
