/*All mocks about all comunications*/

#ifndef _OUR_MPI_H_
#define _OUR_MPI_H_

#ifdef _MOCK_MPI_
#include "main.h"
#define MPI_Init(X, Y) (void)0
#define MPI_Finalize() (void)0
#define MPI_Comm_size(X, Y) *(Y)=4

#define MPI_Bcast_JSON( X )	*(X)=GOOD_JSON;


#ifdef _MOCK_SCHADULER_
#define MPI_Comm_rank(X, Y) *(Y)=0
#endif

#ifdef _MOCK_PRINTER_
#define MPI_Comm_rank(X, Y) *(Y)=2
#endif

#ifdef _MOCK_RAFFLER_
#define MPI_Comm_rank(X, Y) *(Y)=1
#endif

#ifdef _MOCK_NODE_
#define MPI_Comm_rank(X, Y) *(Y)=3
#endif

#ifdef _MOCK_NODE_
#define MPI_Comm_rank(X, Y) *(Y)=3
#endif

#ifdef _QUEUE_NODE_
#define MPI_Comm_rank(X, Y) *(Y)=3
#endif
#ifdef _COMBI_NODE_
#define MPI_Comm_rank(X, Y) *(Y)=3
#endif
#ifdef _NORMAL_NODE_
#define MPI_Comm_rank(X, Y) *(Y)=3
#endif
#ifdef _FUNCTION_NODE_
#define MPI_Comm_rank(X, Y) *(Y)=3
#endif
#ifdef _COUNTER_NODE_
#define MPI_Comm_rank(X, Y) *(Y)=3
#endif

#else 


#include <mpi.h>

#define MPI_Bcast_JSON( A )	MPI_Bcast( A, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD)

#endif

#endif /* our mpi*/
