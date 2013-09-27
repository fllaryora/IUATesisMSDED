#include "ourMPI.h"
#include <stdio.h>
#include <stdlib.h>
#include "raffler.h"
#include "RNGs.h"
#include "main.h"

void raffler(){
	int* combiIds, *currentCombiIds;
	int combiIdsAmount, currentCombiIdsAmount;
	MPI_Status infoComm;
	int currentSource, currentTag;
	int receiverCount = 0;
	int* bufferReceiver = NULL;
	int* bufferSender = NULL;
	
	//obtengo los id de todas las combis del modelo
	ProbeCombiForRaffler( &infoComm );
	GetRafflerCombiCount( &infoComm, &combiIdsAmount );
	combiIds = (int*)malloc(sizeof(int) * combiIdsAmount);
	ReciveRafflerCombi( combiIds, combiIdsAmount );
	
	//seteo la semilla en caso de que se requiera
	//el primer elemento indica si se quiere tener reusabilidad.
	if(combiIds[0] >= 0){
		RandomInitialise(combiIds[0],combiIds[1]);
	}
	combiIdsAmount--;
	combiIds++;
	
	do {
		
		//http://mpitutorial.com/dynamic-receiving-with-mpi-probe-and-mpi-status/
		//Para Probe y MPI_Recv para saber a priori el buffer y fluyar
		//obtengo la orden en currentTag y mi destinatario de respuesta en currentSource 
		ProbeOrderForRaffler(&infoComm);
		currentTag = infoComm.MPI_TAG;
		currentSource = infoComm.MPI_SOURCE;
		GetRafflerOrderCount(&infoComm, &receiverCount);
		if(receiverCount){bufferReceiver = (int*)malloc(sizeof(int) * receiverCount);}
		ReciveRafflerOrder( bufferReceiver, receiverCount );
	
		if (currentTag == NEW_RAFFLE ) {
			Raffle( combiIds ,combiIdsAmount );
			SendRaffleDoneToMaster();
		} else {
			if (currentTag == GET_RAFFLE){
				
				//cambio nomenclatura
				currentCombiIds = bufferReceiver;
				currentCombiIdsAmount = receiverCount;
				//envio a la cola las prioridades del turno
				bufferSender = pickUpOnlySelectedIds(currentCombiIdsAmount, currentCombiIds, combiIdsAmount , combiIds);
				SendRafflePeiorityToQueue(bufferSender, currentCombiIds, currentSource);
				if(bufferSender)free(bufferSender);
			}
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
	for(currentDrawnNumber ; currentDrawnNumber < amountDrawnNumbers;currentDrawnNumber++ ){
		for ( currentSelectedId; currentSelectedId < amountSelected ; currentSelectedId++){
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
void Raffle(int* drawnNumbers, int stakeholderAccount){
	int temp;
	while(stakeholderAccount){
		int newPos = stakeholderAccount > 0? RandomInt(0, stakeholderAccount):0;
		//swap drawnNumbers[stakeholderAccount] with drawnNumbers[newPos]
		temp =  drawnNumbers[newPos];
		drawnNumbers[newPos]  =  drawnNumbers[stakeholderAccount];
		drawnNumbers[stakeholderAccount]  = temp;
		--stakeholderAccount;
	}
}
