#include <mpi.h>
#include <stdio.h>

#define MASTER_ID	0
#define WATCHDOG_DOESNT_BITE_ME(X) (X >= 0)

//TAG is a label of MPI comunication
typedef enum{
	BOOTSTRAP,              /*before send initialization struct*/
	SHUTDOWN,               /*when is reached the live-lock condition or
							the objective of fulfilling certain number 
							of cycles*/
	GENERATION_PHASE,		/* the resources only can in*/
	ADVANCE_PAHSE,			/* the resources only can out*/
	PING_REPORT,            /*when dT end, all nodes must report your 
							status*/
	PONG_REPORT,            /*before send status struct*/
	RESOURCE_SEND,          /*warns when one node has sent a resource to
							another*/

	RESOURCE REQUEST,       /*when a combi want to know about the amount
							of resources containing at a queue.*/
	RESOURCE RESPONSE,      /*when a queue reply about the amount of 
							your resources.*/
	RESOURCE DEMAND,        /*when a combi demand queue's resource.*/

	TRANSACTION BEGIN,      /*warns when a queue has sent a resource to 
							a combi*/
	TRANSACTIONCOMMIT,      /*when a combi accept queue's resource.*/
	TRANSACTION ROLLBACK   /*when a combi refuses queue's resource.*/
}TAG;

//struct for queue initialization
//struct for combi initialization
//struct for function initialization
//struct for counter initialization
//struct for normal initialization


//struct for queue repot
//struct for combi repot
//struct for function repot
//struct for counter repot
//struct for normal repot


/* Local functions */
void scheduler(void);
void genericNode(int myIdNodo);

int localWatchDog(void);

int main(int argc, char **argv){
  int idNodo;

  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  /* Find out my identity in the default communicator */
  MPI_Comm_rank(MPI_COMM_WORLD, &idNodo);
  if (idNodo == MASTER_ID) {
    scheduler();
  } else {
    genericNode(idNodo);
  }
  /* Shut down MPI */
  MPI_Finalize();
  return 0;
}

void scheduler(void){
	int myIdNodo = MASTER_ID;
	int nodesAmount, rank;
	MPI_Status status;
	int watchdog; /*countdown to the live-lock*/
	int dT = 0;
	/* Find out how many processes there are in the default communicator */
	MPI_Comm_size(MPI_COMM_WORLD, &nodesAmount);
	printf("I've %d nodes in the job\n", nodesAmount);
	
}

void genericNode(int myIdNodo)
{
  printf("Hello from slave\n");
}


int localWatchDog(void){
	/*Although the root process and receiver processes do different
	*  jobs, they all call the same MPI_Bcast function.*/
	
	MPI_Bcast(buffer,1,MPI_INT, MASTER_ID, MPI_COMM_WORLD);
}
