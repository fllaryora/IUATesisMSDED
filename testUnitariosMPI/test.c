#include <mpi.h>

#define WORKTAG 1
#define DIETAG 2


/* Local functions */

static void master(void);
static void slave(void);
static unit_of_work_t get_next_work_item(void);
static void process_results(unit_result_t result);
static unit_result_t do_work(unit_of_work_t work);


int main(int argc, char **argv)
{
  int myrank;

  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  /* Find out my identity in the default communicator */

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  if (myrank == 0) {
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
  int ntasks, rank;
  unit_of_work_t work;
  unit_result_t result;
  MPI_Status status;

  /* Find out how many processes there are in the default
     communicator */
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

  /* Seed the slaves; send one unit of work to each slave. */
  for (rank = 1; rank < ntasks; ++rank) {

    /* Find the next item of work to do */
    work = get_next_work_item();

    /* Send it to each rank */
    MPI_Send(&work,             /* message buffer */
             1,                 /* one data item */
             MPI_INT,           /* data item is an integer */
             rank,              /* destination process rank */
             WORKTAG,           /* user chosen message tag */
             MPI_COMM_WORLD);   /* default communicator */
  }

  /* Loop over getting new work requests until there is no more work
     to be done */
  work = get_next_work_item();
  while (work != NULL) {

    /* Receive results from a slave */
    MPI_Recv(&result,           /* message buffer */
             1,                 /* one data item */
             MPI_DOUBLE,        /* of type double real */
             MPI_ANY_SOURCE,    /* receive from any sender */
             MPI_ANY_TAG,       /* any type of message */
             MPI_COMM_WORLD,    /* default communicator */
             &status);          /* info about the received message */

    /* Send the slave a new work unit */
    MPI_Send(&work,             /* message buffer */
             1,                 /* one data item */
             MPI_INT,           /* data item is an integer */
             status.MPI_SOURCE, /* to who we just received from */
             WORKTAG,           /* user chosen message tag */
             MPI_COMM_WORLD);   /* default communicator */

    /* Get the next unit of work to be done */
    work = get_next_work_item();
  }

  /* There's no more work to be done, so receive all the outstanding
     results from the slaves. */
  for (rank = 1; rank < ntasks; ++rank) {
    MPI_Recv(&result, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
             MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  }

  /* Tell all the slaves to exit by sending an empty message with the
     DIETAG. */
  for (rank = 1; rank < ntasks; ++rank) {
    MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
  }
}


static void slave(void)
{
  unit_of_work_t work;
  unit_result_t results;
  MPI_Status status;

  while (1) {

    /* Receive a message from the master */
    MPI_Recv(&work, 1, MPI_INT, 0, MPI_ANY_TAG,
             MPI_COMM_WORLD, &status);

    /* Check the tag of the received message. */
    if (status.MPI_TAG == DIETAG) {
      return;
    }

    /* Do the work */
    result = do_work(work);

    /* Send the result back */
    MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
}

static unit_of_work_t get_next_work_item(void)
{
  /* Fill in with whatever is relevant to obtain a new unit of work
     suitable to be given to a slave. */
}

static void process_results(unit_result_t result)
{
  /* Fill in with whatever is relevant to process the results returned
     by the slave */
}

static unit_result_t
do_work(unit_of_work_t work){
  /* Fill in with whatever is necessary to process the work and
     generate a result */
}
