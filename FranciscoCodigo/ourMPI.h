/*All mocks about all comunications*/

#ifndef _OUR_MPI_H_
#define _OUR_MPI_H_

#ifdef _MOCK_MPI_

//en algun momento tengo que mockear la estructura
typedef struct { 
    int count;
    int MPI_SOURCE;
    int MPI_TAG;
    int MPI_ERROR;
} MPI_Status;

#include "main.h"


#define MPI_Init(X, Y) (void)0
#define MPI_Finalize() (void)0
#define MPI_Comm_size(X, Y) *(Y)=4

//EJECUTADOS POR EL MASTER
#define MPI_Bcast_JSON( X )	*(X)=GOOD_JSON;
#define SendCombisToRaffler(X,Y)	(void)0


//EJECUTADOS POR EL RAFFLER
#define ProbeCombiForRaffler(X)	(void)0
#define GetRafflerCombiCount(X, Y) *(Y)=8
#define ReciveRafflerCombi(X,Y)	for(int i = 0 ; i < Y; i++){ X[i]=i+1; }

//prueba si recibio orden de algun nodo
#define ProbeOrderForRaffler(X)	(X)->MPI_TAG=LIVE_LOCK;(X)->MPI_SOURCE=MASTER_ID;
//descubre el largo del mensaje
#define GetRafflerOrderCount( X, Y)	*(Y)=3
//lee la orden
#define ReciveRafflerOrder(X,Y)	for(int i = 0 ; i < Y; i++){ X[i]=i+1; }
//dice al master que ya sorteo
#define SendRaffleDoneToMaster()	(void)0
//dice a una cola cuales la prioridad del actual sorteo
#define SendRafflePeiorityToQueue(X,Y,Z)	(void)0


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

//broadcastea el resultado de la validacion del json al principio de la arquitectura
#define MPI_Bcast_JSON( A )	MPI_Bcast( A, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD)

// FUNCIONES del RAFFLER
//prueba si recibio semilla mas lista de combis
#define ProbeCombiForRaffler(X)	MPI_Probe( MASTER_ID, SEED_AND_COMBI_LIST, MPI_COMM_WORLD, X)
//descubre el largo de combis en el nodo
#define GetRafflerCombiCount(X, Y)	MPI_Get_count(X, MPI_INT, Y)
//lee la semilla y lista de combis
#define ReciveRafflerCombi(X,Y)	MPI_Recv( X, Y, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
//prueba si recibio orden de algun nodo
#define ProbeOrderForRaffler(X)	MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, X)
//descubre el largo del mensaje
#define GetRafflerOrderCount( X, Y)	MPI_Get_count(X, MPI_INT, Y)
//lee la orden
#define ReciveRafflerOrder(X,Y)	MPI_Recv(X, Y, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
//dice al master que ya sorteo
#define SendRaffleDoneToMaster()	MPI_Send( NULL, 0, MPI_INT, MASTER_ID, RAFFLE_DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
//dice a una cola cuales la prioridad del actual sorteo
#define SendRafflePeiorityToQueue(X,Y,Z)	MPI_Send(X,Y, MPI_INT, Z, RAFFLE_DONE, MPI_COMM_WORLD, MPI_STATUS_IGNORE)



// FUNCIONES del MASTER
#define SendCombisToRaffler(X,Y)	MPI_Send( X , Y , MPI_INT , RAFFLER_ID , SEED_AND_COMBI_LIST , MPI_COMM_WORLD)

#endif

#endif /* our mpi*/
