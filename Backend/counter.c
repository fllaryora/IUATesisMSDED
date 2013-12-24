#include "counter.h"
#include "genericNode.h"


void counterNode( const MPI_Comm commNodes,  const  Counter *initialStatus, const int mpiProcesses){

	int inputResource = 0;//que estan en la entrada antes del cuerpo
	int outputResource = 0; //que cumplieron el dalay se se pueden ir
	unsigned long long int deltaTCount = 0; //cantidad de deltaT que pasaron en este tiempo

	PrinterCounter cReport;
	cReport.idNode = initialStatus->idNode;
	cReport.totalProductivity = 0;
	
	PrinterFinalCounter cReportFinal;
	cReportFinal.idNode = initialStatus->idNode;
	int msg = 0;
	do {
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	
		switch(msg){
			case ADVANCE_PAHSE:
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputResource,outputResource);
				advancePhaseCounter( &inputResource,  &outputResource, initialStatus, commNodes, mpiProcesses, FALSE, &cReport);
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputResource,outputResource);
				
				break;
			case ADVANCE_PAHSE_PRIMA:
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputResource,outputResource);
				advancePhaseCounter( &inputResource,  &outputResource, initialStatus, commNodes, mpiProcesses, TRUE, &cReport);
				//printf("%d: entrada: %d, salida %d\n", initialStatus->idNode,inputResource,outputResource);
				break;
			case GENERATION_PHASE: //hace lo mismo que la de abajo
			case GENERATION_PHASE_PRIMA:
				generationPhaseCounter( &inputResource, &outputResource,  commNodes );
				break;
			case CONSUME_DT:
				deltaTCount++;
				MPI_Barrier( commNodes );
				break;
			case PING_REPORT:
				cReport.productivityPerTime =  cReport.totalProductivity /(double)deltaTCount;			
				MPI_Send(&cReport, sizeof(PrinterCounter), MPI_BYTE, PRINTER_ID, COUNTER_REPORT , MPI_COMM_WORLD);
			default:
				break;
		}
	
	} while (msg != LIVE_LOCK);
	
	cReportFinal.totalProductivity = cReport.totalProductivity;

	MPI_Send(&cReportFinal, sizeof(PrinterFinalCounter), MPI_BYTE, PRINTER_ID, COUNTER_FINAL_REPORT , MPI_COMM_WORLD);
	return;
}

void advancePhaseCounter(int * inputResource, int* outputResource, const Counter *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, PrinterCounter *cReport){ 
	int* bufferReceiver = (int*) malloc( sizeof(int)* initialStatus->countPreceders);
    int receiverCount = 1;
    int msg;
    //inicializo los request de las llegadas de recursos
    MPI_Request* requestPreceders = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countPreceders);
	MPI_Request* requestFollowers = (MPI_Request*) malloc( sizeof(MPI_Request)* initialStatus->countFollowers);
    
	//tomo los envios pendientes del RESOURCE SEND y los paso a la entrada
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		 MPI_Irecv( &bufferReceiver[i], receiverCount, MPI_INT,  initialStatus->preceders[i], RESOURCE_SEND, commNodes, &requestPreceders[i]);
	}
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		 MPI_Isend( outputResource, 1, MPI_INT,  initialStatus->followers[i], RESOURCE_SEND, commNodes, &requestFollowers[i]);
	}

	//espero a que todas la operaciones allan terminado
	for (int i = 0 ; i < initialStatus->countFollowers; i++){
		MPI_Wait(&requestFollowers[i], MPI_STATUS_IGNORE);
		(*outputResource) = 0;
	}
	
	for (int i = 0 ; i < initialStatus->countPreceders; i++){
		MPI_Wait(&requestPreceders[i], MPI_STATUS_IGNORE);
		(*inputResource) += bufferReceiver[i];	
	}
   
	cReport->totalProductivity += (*inputResource)*initialStatus->quantity;
	if( !isPrima ){
		cReport->deltaTProductivity = (*inputResource)*initialStatus->quantity;
		MPI_Barrier( commNodes );
		
	} else {
		int * nodesStatus = NULL;
		cReport->deltaTProductivity += (*inputResource)*initialStatus->quantity;
		msg = (*inputResource)? FALSE: TRUE;
		MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
	}

	free(bufferReceiver);
	free(requestPreceders);
	free(requestFollowers);
	return;
}

void generationPhaseCounter(int* inputResource, int* outputResource ,  const MPI_Comm commNodes ){
	(*outputResource) += (*inputResource);
	(*inputResource) = 0;
	MPI_Barrier( commNodes );
}
