#include <mpi.h>
#include <stdio.h>
#define WORKTAG 1
#define DIETAG 2

/* Local functions */
static void master(void);
static void slave(void);

int main(int argc, char **argv)
{
  int idNodo;

  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  /* Find out my identity in the default communicator */
  MPI_Comm_rank(MPI_COMM_WORLD, &idNodo);
  if (idNodo == 0) {
    master();
  } else {
    slave();
  }

  /* Shut down MPI */
  MPI_Finalize();
  return 0;
}


static void master(void)
{
  int nodes, rank;
  MPI_Status status;

  /* Find out how many processes there are in the default
     communicator */

  MPI_Comm_size(MPI_COMM_WORLD, &nodes);
  printf("I've %d nodes in the job\n", nodes);
 }

static void slave(void)
{
  printf("Hello from slave\n");
}

