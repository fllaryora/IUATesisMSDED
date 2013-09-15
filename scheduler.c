#include <mpi.h>
#include <stdio.h>
#include "scheduler.h"
#include "RNGs.h"

#define WATCHDOG_DOESNT_BITE_ME(X) (X >= 0)

//--MAIN---
void scheduler(void){
	MPI_Status status;
	int myIdNodo = MASTER_ID;
	int slaveNodes; //amount of slave nodes
	int watchdog; /*countdown to the live-lock*/
	int dT = 0;
	/* Find out how many processes there are in the default communicator */
	MPI_Comm_size(MPI_COMM_WORLD, &slaveNodes);
	--slaveNodes; //except me
	
	printf("I've %d nodes in the job\n", nodesAmount);
	
}

/* 
   Return the jackpot in drawnNumbers[stakeholderAccount -1]
*/
void Raffle(int* drawnNumbers, int stakeholderAccount){
	int temp;
	int newPos = stakeholderAccount > 0?RandomInt(0, stakeholderAccount):0;
	//swap drawnNumbers[stakeholderAccount] with drawnNumbers[newPos]
	temp =  drawnNumbers[newPos];
	drawnNumbers[newPos]  =  drawnNumbers[stakeholderAccount];
	drawnNumbers[stakeholderAccount]  = temp;	
}


