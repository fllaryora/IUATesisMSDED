#include "ourMPI.h"
#include "main.h"
#include <stdio.h>
#include "scheduler.h"


#define WATCHDOG_DOESNT_BITE_ME(X) (X >= 0)

//--MAIN---
void scheduler(void){
	int slaveNodes;
	/* Find out how many processes there are in the default communicator */
	MPI_Comm_size(MPI_COMM_WORLD, &slaveNodes);
	slaveNodes -= MASTER_RAFFLER_PRINTER; //except nodes helpers
	printf("I've %d nodes in the job\n", slaveNodes);
	
	//supongamos que comienza las faces de nuevo
	NewRaffle();
	
	
}


