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

#define MPI_Comm_group(Z, X)        *(X)=0
#define MPI_Group_incl(X, Y, Z, W)        *(W)=0
#define MPI_Comm_create(Z, X, Y)        *(Y)=0
//Seteo que quiero leer los errores
#define MPI_Errhandler_set(X, Z)        (void)0
//retorna la clase de error en un string
#define MPI_Error_class(X, Y)        *(Y)=4
//retorna el error en un string
#define MPI_Error_string(X, Y, Z)        Y[0]='E';Y[1]='R';Y[2]='R';Y[3]='O';Y[4]='R';Y[5]='\0'
//aborta MPI
#define MPI_Abort(Z, X)        exit(X)
#define MPI_Send( X , Y , Z , W , A , B)        (void)0

//EJECUTADOS POR EL MASTER
#define MPI_Bcast_JSON( X, Q, W, Y, Z)        *(X)=GOOD_JSON

#define MPI_Get_count(X, Z, Y)	*(Y)=10
#define MPI_Get_count1(X, Z, Y)	*(Y)=4
#define MockAlRaffle(X)        X=GET_RAFFLE
#define MockAlLive(X)        X=LIVE_LOCK
#define MockLoop(STTUS)        (STTUS)->MPI_TAG=LIVE_LOCK
#define MockResult(STTUS)	(STTUS)->MPI_TAG=PRINT_SIGNAL;(STTUS)->MPI_SOURCE=MASTER_ID
#define MPI_Probe( A, B, C, X)        (X)->MPI_TAG=NEW_RAFFLE;(X)->MPI_SOURCE=MASTER_ID

//recive arreglo de enteros desde el uno
#define MPI_Recv1( X, Y, Z, A, B, C, D)        for(int i = 0 ; i < Y; i++){ X[i]=i+1; }(void)0
#define MPI_Recv12( X, Y, Z, A, B, C, D)        for(int i = 0 ; i < Y; i++){ X[i]=i+2; }(void)0
//recive un arreglo de todos unos
#define MPI_Recv2( X, Y, Z, A, B, C, D)        for(int i = 0 ; i < Y; i++){ X[i]=1; }(void)0
//creo una estructura llana de ceros
#define MPI_Recv3(X, Y, Z, W, TAG , R, T)        memset (X, 0,Y )
//creo una estructura llana de ceros y Pongo en la estructura el origen
#define MPI_Recv4(X, Y, Z, W, TAG , R, T)        memset (X, 0,Y );(T)->MPI_SOURCE=FIST_NODE_ID
//recive un arreglo de todos 4.5
#define MPI_Recv5( X, Y, Z, A, B, C, D)        for(int i = 0 ; i < Y; i++){ X[i]=4.5; }(void)0
#define MPI_Recv6( X, Y, Z, A, B, C, D)        *(X)=0.5

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

#define MPI_Recv1	MPI_Recv
#define MPI_Recv2	MPI_Recv
#define MPI_Recv3	MPI_Recv
#define MPI_Recv4	MPI_Recv
#define MPI_Recv5	MPI_Recv
#define MPI_Recv6	MPI_Recv
#define MPI_Recv12	MPI_Recv

#define MPI_Bcast_JSON	MPI_Bcast

//dice al master que ya sorteo
#define SendRaffleDoneToMaster()        (void)0

//forzar sincronizacion
#define MPI_Send	MPI_Ssend

// FUNCIONES del MASTER

// FUNCIONES del PRINTER
//no hace nada
#define MockAlRaffle(X)	(void)0
#define MockLoop(STTUS)        (void)0
#define MockAlLive(X)        (void)0
#define MockResult(STTUS)	(void)0
#endif

#endif /* our mpi*/
