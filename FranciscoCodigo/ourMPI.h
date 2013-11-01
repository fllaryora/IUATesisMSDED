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

#define MPI_Group	int
#define MPI_Comm	int

#define MPI_SUCCESS	0
#define MPI_MAX_ERROR_STRING	20
#define MPI_COMM_WORLD	0
#define MPI_ERRORS_RETURN	0
#define MPI_ERRORS_ARE_FATAL	0
#define MPI_INT	0
#define MPI_DOUBLE	0
#define MPI_ANY_SOURCE	0
#define MPI_ANY_TAG	0
#define MPI_STATUS_IGNORE	0
#define MPI_ERRORS_ARE_FATAL	0
#define MPI_ERRORS_ARE_FATAL	0
#define MPI_ERRORS_ARE_FATAL	0
#define MPI_ERRORS_ARE_FATAL	0
#define MPI_ERRORS_ARE_FATAL	0

#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MPI_Init(X, Y) (void)0
#define MPI_Finalize() (void)0
#define MPI_Comm_size(X, Y) *(Y)=4

#define MPI_Comm_group(MPI_COMM_WORLD, X)        *(X)=0
#define MPI_Group_incl(X, Y, Z, W)        *(W)=0
#define MPI_Comm_create(MPI_COMM_WORLD, X, Y)        *(Y)=0
//Seteo que quiero leer los errores
#define MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN)        (void)0
//Seteo que no voy leer los errores
#define MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL)        (void)0
//retorna la clase de error en un string
#define MPI_Error_class(X, Y)        *(Y)=4
//retorna el error en un string
#define MPI_Error_string(X, Y, Z)        Y[0]='E';Y[1]='R';Y[2]='R';Y[3]='O';Y[4]='R';Y[5]='\0'
//aborta MPI
#define MPI_Abort(MPI_COMM_WORLD, X)        exit(X)

//EJECUTADOS POR EL MASTER
#define MPI_Bcast( X, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD)        *(X)=GOOD_JSON
#define MPI_Send( X , Y , MPI_INT , RAFFLER_ID , SEED_AND_COMBI_LIST , MPI_COMM_WORLD)        (void)0
#define MPI_Send( NULL , 0 , MPI_INT , RAFFLER_ID , NEW_RAFFLE , MPI_COMM_WORLD)        (void)0
#define MPI_Send( NULL , 0 , MPI_INT , RAFFLER_ID , LIVE_LOCK , MPI_COMM_WORLD)        (void)0
#define MPI_Send(TIME, 1, MPI_DOUBLE, MPI_INT , PRINTER_ID , LIVE_LOCK , MPI_COMM_WORLD)        (void)0

//EJECUTADOS POR EL RAFFLER
#define MPI_Probe( MASTER_ID, SEED_AND_COMBI_LIST, MPI_COMM_WORLD, X)        (void)0
#define MPI_Get_count(X, MPI_INT, Y)	*(Y)=10
//-1,0,1,2,3,4,5,6,7,8
//pero sacando el inicio
//0,1,2,3,4,5,6,7,8
#define MPI_Recv( X, Y, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)        for(int i = 0 ; i < Y; i++){ X[i]=i+1; }(void)0
//prueba si recibio orden de algun nodo
#define MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, X)        (X)->MPI_TAG=NEW_RAFFLE;(X)->MPI_SOURCE=MASTER_ID
//descubre el largo del mensaje
#define MPI_Get_count1(X, MPI_INT, Y)	*(Y)=4
//lee la orden
//1,2,3,4
//lo mismo
//dice al master que ya sorteo
#define MockAlRaffle(X)        X=GET_RAFFLE
#define MockAlLive(X)        X=LIVE_LOCK

//dice a una cola cuales la prioridad del actual sorteo
#define MPI_Send(X,Y, MPI_INT, Z, RAFFLE_DONE, MPI_COMM_WORLD)        currentTag = LIVE_LOCK

// EJECUTADOS POR EL PRINTER
#define MPI_Recv(X, 5, MPI_INT, MASTER_ID, INIT_NODES , MPI_COMM_WORLD, MPI_STATUS_IGNORE)	for(int i = 0 ; i < 5; i++){ X[i]=1; }(void)0
#define MPI_Recv(STRU, SIZE, MPI_BYTE, MPI_ANY_SOURCE, TAG , MPI_COMM_WORLD, MPI_STATUS_IGNORE)        memset (STRU, 0,SIZE )
#define MPI_Recv(STRU, SIZE, MPI_BYTE, MPI_ANY_SOURCE, TAG , MPI_COMM_WORLD, STTUS)         memset (STRU, 0,SIZE );(STTUS)->MPI_SOURCE=FIST_NODE_ID
#define MPI_Recv(DLY, SIZE, MPI_DOUBLE, NODE_ID, TAG , MPI_COMM_WORLD, STTUS)        for(int i = 0 ; i < SIZE; i++){ DLY[i]=4.5; }(void)0
#define MPI_Recv(TIME, 1, MPI_DOUBLE, MASTER_ID, MPI_ANY_TAG , MPI_COMM_WORLD, STTUS)         *(TIME)=0.5;(STTUS)->MPI_TAG=PRINT_SIGNAL;(STTUS)->MPI_SOURCE=MASTER_ID
#define MockLoop(STTUS)        (STTUS)->MPI_TAG=LIVE_LOCK

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

//descubre el largo del mensaje
#define MPI_Get_count1	MPI_Get_count

//dice al master que ya sorteo
#define SendRaffleDoneToMaster()        (void)0

// FUNCIONES del MASTER

// FUNCIONES del PRINTER
//no hace nada
#define MockLoop(STTUS)        (void)0

#endif

#endif /* our mpi*/