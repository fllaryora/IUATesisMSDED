/*
 * BotQueue and all modules are free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
#ifndef _MAIN_H_
#define _MAIN_H_

#define MASTER_ID	0
#define RAFFLER_ID	1
#define PRINTER_ID	2
#define MASTER_RAFFLER_PRINTER	3

//TAG is a label of MPI comunication
typedef enum{
	BAD_JSON,
	GOOD_JSON,
	
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

typedef enum{
	QUEUE, NORMAL, COUNTER, FUNCTION, COMBI
}TYPENODE;


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
}Deterministic;

typedef struct{
	int semilla;
	float minimo;
	float maximo;
}Uniform;

typedef struct{
	int semilla;
	float media;
	float varianza;
}Gaussian;

typedef struct{
	int semilla;
	float lambda;
}Exponential;

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


typedef enum{
	DETERMINISTIC, UNIFORM, GAUSSIAN, EXPONENTIAL, TRIANGULAR, BETA, LOGNORMAL
}DELAY_TYPE;

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

typedef struct{
	Total enTotal;
	EnDt enElDt;
}Report;
#endif /* #ifndef _MAIN_H_*/
