#include <mpi.h>
#include <stdio.h>
#include "scheduler.h"
#include "RNGs.h"

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
	RESOURCE_REQUEST,       /*when a combi want to know about the amount
							of resources containing at a queue.*/
	RESOURCE_RESPONSE,      /*when a queue reply about the amount of 
							your resources.*/
	RESOURCE_DEMAND,        /*when a combi demand queue's resource.*/
	RESOURCE_NO_DEMAND,     /*when a combi tell to queue's i'm do not demand the resource.*/
	TRANSACTION_BEGIN,      /*warns when a queue has sent a resource to 
							a combi*/
	TRANSACTION_COMMIT,      /*when a combi accept queue's resource.*/
	TRANSACTION_ROLLBACK   /*when a combi refuses queue's resource.*/
}TAG;

//nodo generico
typedef struct{
	int tipo; //-->enum
	int idnodo;
	int recursos;
	int antecesores[nodesInModel];
	int sucesor[nodesInModel];
	float costoFijo;
	float costoVariable;
	int multiplicador;
	int entrada;
	int salida;
	int salidaProbabilistica;
	float probabilidadPorSalida[nodesInModel];
	int distribucion; //--->enum
	int constante;
	int semilla;
	float minimo;
	float maximo;
	float lambda;
	float moda;
	float alfa;
	float beta;
	float media;
	float varianza;
	float desviacionEstandar;
}Generic;

//initilization structs
typedef struct{
	int idnodo;
	int recursos;
	int antecesoresCount;
	int* antecesores; //arreglo
	int sucesoresCount;
	int* sucesores;
	float costoFijo;
	float costoVariable;
}Queue;
			
typedef struct{
	int idnodo;
	int multiplicador;
	int antecesoresCount;
	int* antecesores; //arreglo
	int sucesoresCount;
	int* sucesores;
}Counter;

typedef struct{
	int idnodo;
	int entrada;
	int salida;
	int salidaProbabilistica; //boleano
	int probabilidadPorSalidasConter;
	float* probabilidadPorSalidas;
	int antecesoresCount;
	int* antecesores; //arreglo
	int sucesoresCount;
	int* sucesores;
}Function;

typedef struct{
	int idnodo;
	int salidaProbabilistica;
	int probabilidadPorSalidasConter;
	float* probabilidadPorSalidas;
	int antecesoresCount;
	int* antecesores; //arreglo
	int sucesoresCount;
	int* sucesores;
}Normal;

typedef struct{
	int idnodo;
	int salidaProbabilistica;
	int probabilidadPorSalidasConter;
	float* probabilidadPorSalidas;
	int antecesoresCount;
	int* antecesores; //arreglo
	int sucesoresCount;
	int* sucesores;
}Combi;

typedef struct{
	int constante;
}Deterministica;

typedef struct{
	int semilla;
	float minimo;
	float maximo;
}Uniforme;

typedef struct{
	int semilla;
	float media;
	float varianza;
}Gaussiana;

typedef struct{
	int semilla;
	float lambda;
}Exponencial;

typedef struct{
	int semilla;
	float minimo;
	float maximo;
	float moda;
}Triangular;

typedef struct{
	int semilla;
	float minimo;
	float maximo;
	float alfa;
	float beta;
}Beta;

typedef struct{
	int semilla;
	float minimo;
	float media;
	float desviacionEstandar;
}Lognormal;


//struct for queue repot
//struct for combi repot
//struct for function repot
//struct for counter repot
//struct for normal repot


//--MAIN---
void scheduler(void){
	MPI_Status status;
	int myIdNodo = MASTER_ID;
	int slaveNodes; //amount of slave nodes
	int watchdog; /*countdown to the live-lock*/
	int dT = 0;
	/* Find out how many processes there are in the default communicator */
	MPI_Comm_size(MPI_COMM_WORLD, &slaveNodes);
	--slaveNodes; //except me
	
	printf("I've %d nodes in the job\n", nodesAmount);
	
}

/* 
   Return the jackpot in drawnNumbers[stakeholderAccount -1]
*/
void Raffle(int* drawnNumbers, int stakeholderAccount){
	int temp;
	int newPos = stakeholderAccount > 0?RandomInt(0, stakeholderAccount):0;
	//swap drawnNumbers[stakeholderAccount] with drawnNumbers[newPos]
	temp =  drawnNumbers[newPos];
	drawnNumbers[newPos]  =  drawnNumbers[stakeholderAccount];
	drawnNumbers[stakeholderAccount]  = temp;	
}


