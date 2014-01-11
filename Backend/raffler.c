#include "ourMPI.h"
#include <stdio.h>
#include <stdlib.h>
#include "raffler.h"
#include "RNGs.h"
#include "main.h"

//http://mpitutorial.com/dynamic-receiving-with-mpi-probe-and-mpi-status/
//Para Probe y MPI_Recv para saber a priori el buffer y fluyar
void raffler(){
	int* combiIds = NULL, *currentCombiIds = NULL;
	int combiIdsAmount, currentCombiIdsAmount;
	MPI_Status infoComm;
	int currentSource, currentTag;
	int receiverCount = 0;
	int* bufferReceiver = NULL;
	int* bufferSender = NULL;
	
	RngInstance rngCombiDrawn;
	rngCombiDrawn.isInitialise = FALSE;

	//obtengo los id de todas las combis del modelo
	MPI_Probe( MASTER_ID, SEED_AND_COMBI_LIST, MPI_COMM_WORLD,  &infoComm );
	//"el sizeof" de lo que viene
	MPI_Get_count(&infoComm, MPI_INT, &combiIdsAmount );
	combiIds = (int*)malloc(sizeof(int) * combiIdsAmount);
	MPI_Recv1( combiIds, combiIdsAmount, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
	//seteo la semilla en caso de que se requiera
	//el primer elemento indica si se quiere tener reusabilidad.
	if(combiIds[0] >= 0){
		RandomInitialise(&rngCombiDrawn,combiIds[0],combiIds[0]);
	}
	combiIdsAmount--;
	combiIds++;
	//printf("Orden de los ids original\n");
	//for(int i = 0; i < combiIdsAmount ; i++){ printf("%d ",combiIds[i]);}
	
	
	do {
		
		//obtengo la orden en currentTag y mi destinatario de respuesta en currentSource 
		MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &infoComm);
		currentTag = infoComm.MPI_TAG;
		//printf("TAG = %d\n", currentTag);
		currentSource = infoComm.MPI_SOURCE;
		MPI_Get_count1( &infoComm, MPI_INT, &receiverCount);
		if( receiverCount == 0 ) receiverCount = 1;
		//printf("receiverCount = %d\n", receiverCount);
		bufferReceiver = (int*)malloc(sizeof(int) * receiverCount);
		MPI_Recv12( bufferReceiver, receiverCount, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
		if (currentTag == NEW_RAFFLE ) {
			
			Raffle( combiIds ,combiIdsAmount , &rngCombiDrawn);
			//printf("Orden de los ids mesclados\n");
			//for(int i = 0; i < combiIdsAmount ; i++){
			//	 printf("%d ",combiIds[i]);
			//}
			//printf("\n");

			MockAlRaffle(currentTag); //currentTag=GET_RAFFLE
		} 
		
		if (currentTag == GET_RAFFLE){
			
			//cambio nomenclatura
			currentCombiIds = bufferReceiver;
			currentCombiIdsAmount = receiverCount;
			//printf("Los id de combis de la cola\n");
			//for(int i = 0; i < currentCombiIdsAmount ; i++){ printf("%d ",currentCombiIds[i]);}
			//printf("\n");
			
			//envio a la cola las prioridades del turno
			bufferSender = pickUpOnlySelectedIds(currentCombiIdsAmount, currentCombiIds, combiIdsAmount , combiIds);
			//printf("Lo que se va ha enviar\n");
			//for(int i = 0; i < currentCombiIdsAmount ; i++){ printf("%d ",bufferSender[i]);}
			//printf("\n");
			//Envio la priotidad a la cola
			MPI_Send(bufferSender, currentCombiIdsAmount, MPI_INT, currentSource, RAFFLE_DONE, MPI_COMM_WORLD);
			MockAlLive(currentTag); //currentTag=LIVE_LOCK
			if(bufferSender)free(bufferSender);
		}
		
		if(bufferReceiver)free(bufferReceiver);
		bufferReceiver = NULL;
		bufferSender = NULL;
		receiverCount = 0;
	
	} while (currentTag != LIVE_LOCK);
	free(--combiIds);
}


/* toma los id seleccionados de la lista por orden de sorteados, que esten en la lista de combis en la cola que lo demando*/
int* pickUpOnlySelectedIds(int amountSelected, int* selectedId, int amountDrawnNumbers , int* drawnNumbers){
	int currentPosition = 0;
	int currentSelectedId, currentDrawnNumber;	
	
	int* idPickUpted = (int*)malloc(sizeof(int) * amountSelected);
	for(currentDrawnNumber = 0; currentDrawnNumber < amountDrawnNumbers;currentDrawnNumber++ ){
		for ( currentSelectedId = 0; currentSelectedId < amountSelected ; currentSelectedId++){
			if(selectedId[currentSelectedId] == drawnNumbers[currentDrawnNumber]){
				idPickUpted[currentPosition++] = drawnNumbers[currentDrawnNumber];
				break;
			}
		}
	}
	return idPickUpted;
}

/* 
   Revuelve la lista de combis id contra el ultimo elemento de la lista, para sortear
*/
void Raffle(int* drawnNumbers, int stakeholderAccount, RngInstance* rngCombiDrawn){
	int temp;
	stakeholderAccount--;
	while(stakeholderAccount){
		int newPos =  RandomInt(rngCombiDrawn,0, stakeholderAccount);
		//swap drawnNumbers[stakeholderAccount] with drawnNumbers[newPos]
		temp =  drawnNumbers[newPos];
		drawnNumbers[newPos]  =  drawnNumbers[stakeholderAccount];
		drawnNumbers[stakeholderAccount]  = temp;
		--stakeholderAccount;
	}
}
