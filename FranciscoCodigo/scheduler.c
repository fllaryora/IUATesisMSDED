#include "ourMPI.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"


#define WATCHDOG_DOESNT_BITE_ME(X) (X >= 0)


void scheduler(void){

	printf("Hello from master\n");
	//supongamos que comienza las faces de nuevo
	NewRaffle();
	
}


