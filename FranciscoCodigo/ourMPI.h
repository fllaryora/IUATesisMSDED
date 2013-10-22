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
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MPI_Init(X, Y) (void)0
#define MPI_Finalize() (void)0
#define MPI_Comm_size(X, Y) *(Y)=4

#define GetCommWorldHandle(X)	*(X)=0
#define CreateGroupByIds(X, Y, Z, W)	*(W)=0
#define CreateCommByGroup(X, Y)	*(Y)=0
//Seteo que quiero leer los errores
#define setLoger()	(void)0
//Seteo que no voy leer los errores
#define unsetLoger()	(void)0
//retorna la clase de error en un string
#define getErrorCalss(X, Y)	*(Y)=4
//retorna el error en un string
#define getErrorString(X, Y, Z)	Y[0]='E';Y[1]='R';Y[2]='R';Y[3]='O';Y[4]='R';Y[5]='\0'
//aborta MPI
#define abortAllProcess(X)	exit(X)

//EJECUTADOS POR EL MASTER
#define MPI_Bcast_JSON( X )	*(X)=GOOD_JSON
#define SendCombisToRaffler(X,Y)	(void)0
#define NewRaffle()	(void)0
#define SendLiveLockToRaffler()	(void)0
#define SendLiveLockToPrinter()	(void)0

//EJECUTADOS POR EL RAFFLER
#define ProbeCombiForRaffler(X)	(void)0
#define GetRafflerCombiCount(X, Y) *(Y)=10
//-1,0,1,2,3,4,5,6,7,8
//pero sacando el inicio
//0,1,2,3,4,5,6,7,8
#define ReciveRafflerCombi(X,Y)	for(int i = 0 ; i < Y; i++){ X[i]=i+1; }(void)0
//prueba si recibio orden de algun nodo
#define ProbeOrderForRaffler(X)	(X)->MPI_TAG=NEW_RAFFLE;(X)->MPI_SOURCE=MASTER_ID
//descubre el largo del mensaje
#define GetRafflerOrderCount( X, Y)	*(Y)=4
//lee la orden
//1,2,3,4
#define ReciveRafflerOrder(X,Y)	for(int i = 0 ; i < Y; i++){ X[i]=i+1; }(void)0
//dice al master que ya sorteo
#define SendRaffleDoneToMaster()	currentTag = GET_RAFFLE
//dice a una cola cuales la prioridad del actual sorteo
#define SendRafflePeiorityToQueue(X,Y,Z)	currentTag = LIVE_LOCK



// EJECUTADOS POR EL PRINTER
#define GetEachNodesAmount(X)		for(int i = 0 ; i < 5; i++){ X[i]=1; }(void)0
#define GetNodeStruct(STRU, SIZE, TAG)	memset (STRU, 0,SIZE )
#define GetActivityStruct(STRU, SIZE, TAG, STTUS)	 memset (STRU, 0,SIZE );(STTUS)->MPI_SOURCE=FIST_NODE_ID
#define GetDelayActivityArrayStruct(DLY, SIZE, TAG, NODE_ID)	for(int i = 0 ; i < SIZE; i++){ DLY[i]=4.5; }(void)0


//recive una estructura sin importarle la fuente
#define GetNodeStruct(STRU, SIZE, TAG)	MPI_Recv(STRU, SIZE, MPI_BYTE, MPI_ANY_SOURCE, TAG , MPI_COMM_WORLD, MPI_STATUS_IGNORE)
//recive una estructura sin importarle la fuente, pero la debe recordar para mas adelante
#define GetActivityStruct(STRU, SIZE, TAG, STTUS)	MPI_Recv(STRU, SIZE, MPI_BYTE, MPI_ANY_SOURCE, TAG , MPI_COMM_WORLD, STTUS)
//cuanto delay tiene cada actividad dentro de un activity
#define GetDelayActivityArrayStruct(DLY, SIZE, TAG, NODE_ID)	MPI_Recv(DLY, SIZE, MPI_DOUBLE, NODE_ID, TAG , MPI_COMM_WORLD, STTUS)

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

//Seteo que quiero leer los errores
#define setLoger()	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN)
//Seteo que no voy leer los errores
#define unsetLoger()	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL)
//retorna la clase de error en un string
#define getErrorCalss(X, Y)	MPI_Error_class(X, Y)
//retorna el error en un string
#define getErrorString(X, Y, Z)	MPI_Error_string(X, Y, Z)
//aborta MPI
#define abortAllProcess(X)	MPI_Abort(MPI_COMM_WORLD, X)


//broadcastea el resultado de la validacion del json al principio de la arquitectura
#define MPI_Bcast_JSON( A )	MPI_Bcast( A, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD)
//busco el handle del comm world y se lo paso a un grupo
#define GetCommWorldHandle(X)	MPI_Comm_group(MPI_COMM_WORLD, X)
//creo grupo by process
#define CreateGroupByIds(X, Y, Z, W)	MPI_Group_incl(X, Y, Z, W)
// creo comunicador by group
#define CreateCommByGroup(X, Y)	MPI_Comm_create(MPI_COMM_WORLD, X, Y)


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
#define SendRaffleDoneToMaster()	(void)0
//dice a una cola cuales la prioridad del actual sorteo
#define SendRafflePeiorityToQueue(X,Y,Z)	MPI_Send(X,Y, MPI_INT, Z, RAFFLE_DONE, MPI_COMM_WORLD)



// FUNCIONES del MASTER
#define SendCombisToRaffler(X,Y)	MPI_Send( X , Y , MPI_INT , RAFFLER_ID , SEED_AND_COMBI_LIST , MPI_COMM_WORLD)
#define NewRaffle()	MPI_Send( NULL , 0 , MPI_INT , RAFFLER_ID , NEW_RAFFLE , MPI_COMM_WORLD)
#define SendLiveLockToRaffler()	MPI_Send( NULL , 0 , MPI_INT , RAFFLER_ID , LIVE_LOCK , MPI_COMM_WORLD)
#define SendLiveLockToPrinter()	MPI_Send( NULL , 0 , MPI_INT , PRINTER_ID , LIVE_LOCK , MPI_COMM_WORLD)
#endif



// FUNCIONES del PRINTER
//cuantos nodos hay de cada uno
#define GetEachNodesAmount(X)	MPI_Recv(X, 5, MPI_INT, MASTER_ID, INIT_NODES , MPI_COMM_WORLD, MPI_STATUS_IGNORE)
//recive una estructura sin importarle la fuente
#define GetNodeStruct(STRU, SIZE, TAG)	MPI_Recv(STRU, SIZE, MPI_BYTE, MPI_ANY_SOURCE, TAG , MPI_COMM_WORLD, MPI_STATUS_IGNORE)
//recive una estructura sin importarle la fuente, pero la debe recordar para mas adelante
#define GetActivityStruct(STRU, SIZE, TAG, STTUS)	MPI_Recv(STRU, SIZE, MPI_BYTE, MPI_ANY_SOURCE, TAG , MPI_COMM_WORLD, STTUS)
//cuanto delay tiene cada actividad dentro de un activity
#define GetDelayActivityArrayStruct(DLY, SIZE, TAG, NODE_ID)	MPI_Recv(DLY, SIZE, MPI_DOUBLE, NODE_ID, TAG , MPI_COMM_WORLD, STTUS)

#endif /* our mpi*/
