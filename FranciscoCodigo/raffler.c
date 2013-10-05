#include "ourMPI.h"
#include <stdio.h>
#include "raffler.h"
#include "jsonHelper.h"
#include "RNGs.h"
#include "main.h"

void raffler(){
	
	int numberAmount;
	MPI_Status infoComm;
	int* bufferReceiver;
	MPI_Status status;
	int* bufferSender;
	int* combiIds;
	int currentSource, currentTag;
	
	combiIds = getCombiIds();
	
	do {
		//http://mpitutorial.com/dynamic-receiving-with-mpi-probe-and-mpi-status/
		MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &infoComm);
		currentTag = infoComm.MPI_TAG;
		currentSource = infoComm.MPI_SOURCE;
		MPI_Get_count(&infoComm, MPI_INT, &numberAmount);
		bufferReceiver = (int*)malloc(sizeof(int) * numberAmount);
		MPI_Recv(bufferReceiver, numberAmount, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if (currentTag == NEW_RAFFLE ) {
			Raffle( &combiIds[1] ,combiIds[0] );
			MPI_Send(&combiIds[0], 0, MPI_INT, currentSource, NEW_RAFFLE_DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		} else {
			if (currentTag != SHUTDOWN){
				bufferSender = pickUpOnlySelectedIds(numberAmount, bufferReceiver, combiIds[0] , &combiIds[1]);
				MPI_Send(bufferSender, numberAmount, MPI_INT, currentSource, NEW_RAFFLE_DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				free(bufferSender);
			}
		}
		free(bufferReceiver);
	} while (currentTag != SHUTDOWN);
	free(combiIds);
}

int* pickUpOnlySelectedIds(int amountSelected, int* selectedId, int amountDrawnNumbers , int* drawnNumbers){
	int currentPosition = 0;
	int currentSelectedId, currentDrawnNumber;
	int* idPickUpted = (int*)malloc(sizeof(int) * amountSelected);
	for ( currentSelectedId; currentSelectedId < amount ; currentSelectedId++){
		for(currentDrawnNumber ; currentDrawnNumber < amountDrawnNumbers;currentDrawnNumber++ ){
			if(selectedId[currentSelectedId] == drawnNumbers[currentDrawnNumber]){
				idPickUpted[currentPosition++] = drawnNumbers[currentDrawnNumber];
				break;
			}
		}
	}
	return idPickUpted;
}

/* 
   Return the jackpot in drawnNumbers[stakeholderAccount -1]
*/
void Raffle(int* drawnNumbers, int stakeholderAccount){
	int temp;
	while(stakeholderAccount){
		int newPos = stakeholderAccount > 0?RandomInt(0, stakeholderAccount):0;
		//swap drawnNumbers[stakeholderAccount] with drawnNumbers[newPos]
		temp =  drawnNumbers[newPos];
		drawnNumbers[newPos]  =  drawnNumbers[stakeholderAccount];
		drawnNumbers[stakeholderAccount]  = temp;
		--stakeholderAccount;
	}
}


