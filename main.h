#ifndef _MAIN_H_
#define _MAIN_H_

//TAG is a label of MPI comunication
typedef enum{
	BOOTSTRAP,              /*before send initialization struct*/
	SHUTDOWN,               /*when is reached the live-lock condition or
							the objective of fulfilling certain number 
							of cycles*/
							
	//SCATTER enviada de estructura de estructura
	
							
	GENERATION_PHASE,		/* the resources only can in and procese*/
	ADVANCE_PAHSE,			/* the resources only can out*/
	GENERATION_PHASE_PRIMA,	/* the resources only can in but can't
							procese*/
	ADVANCE_PAHSE_PRIMA,	/* the resources only can out and tell to 
							scheduler if he terminate */
	
	
	
	PING_REPORT,            /*when dT end, all nodes must report your 
							status*/
	CONSUME_DT,            /*when dT end, all nodes consume one dT*/
	NEW_RAFFLE,			   /*when dT end, and the jackpot of combi's Id 
						   must change*/
	GET_RAFFLE,			   /*when dT end, i need the jackpot of combi's
						   Id*/					   
	PRINT_SIGNAL, 		   /*when dT end, printer must write another 
						   dT story*/
	//GATHER enviada de reporte de dT
	
	
	
	RESOURCE_REQUEST,       /*when a combi want to know about the amount
							of resources containing at a queue.*/
	RESOURCE_RESPONSE,      /*when a queue reply about the amount of 
							your resources.+amount*/
	RESOURCE_DEMAND,        /*when a combi demand queue's resource.+ID*/
	RESOURCE_NO_DEMAND,     /*when a combi tell to queue's i'm do not 
							demand the resource.+ID*/
	TRANSACTION_BEGIN,      /*warns when a queue has sent a resource to 
							a combi*/
	TRANSACTION_CANCELLED,  /*warns when a queue can't send a resource 
							to a combi*/						
	TRANSACTION_COMMIT,      /*when a combi accept queue's resource.*/
	TRANSACTION_ROLLBACK,   /*when a combi refuses queue's resource.*/
	
	RESOURCE_SEND          /*warns when one node has sent a resource to
							another*/	
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

typedef struct{
	Total enTotal;
	EnDt enElDt;
}Report;

typedef union{
	int productividad;
	int recursos;
	int workTasks;
}Total;

typedef union{
	int productividad;
	int recursos;
	int workTasks;
}EnDt;

#endif /* #ifndef _MAIN_H_*/
