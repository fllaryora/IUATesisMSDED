#include "ourMPI.h"
#include <stdio.h>
#include "scheduler.h"


#define WATCHDOG_DOESNT_BITE_ME(X) (X >= 0)

//--MAIN---
void scheduler(void){
	int slaveNodes;
	/* Find out how many processes there are in the default communicator */
	MPI_Comm_size(MPI_COMM_WORLD, &slaveNodes);
	slaveNodes -= 3; //except nodes helpers
	printf("I've %d nodes in the job\n", slaveNodes);
	
}


