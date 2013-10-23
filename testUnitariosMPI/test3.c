#include <mpi.h>
#include <stdio.h>

#define MASTER_ID	0

/* Local functions */
void scheduler(void);
void genericNode(int myIdNodo);

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

/* Find out how many processes there are in the default communicator */
MPI_Comm_size(MPI_COMM_WORLD, &nodesAmount);
printf("I've %d nodes in the job\n", nodesAmount);

}

void genericNode(int myIdNodo)
{
  printf("Hello from slave\n");
}
