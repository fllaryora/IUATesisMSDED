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

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#define MASTER_ID	0
#define RAFFLER_ID	1
#define PRINTER_ID	2
#define FIST_NODE_ID	3
#define	RAFFLER_PRINTER	2
#define MASTER_RAFFLER_PRINTER	3

//TAG is a label of MPI comunication
typedef enum{
	
	BAD_JSON,
	GOOD_JSON,
	
	LIVE_LOCK,               /*when is reached the live-lock condition or
							the objective of fulfilling certain number 
							of cycles*/
	NEW_RAFFLE,			   /*when dT end, and the jackpot of combi's Id 
						   must change*/
	RAFFLE_DONE,
	SEED_AND_COMBI_LIST,   /* cuando el scheduler le manda la lista de combis al raffer*/
	GET_RAFFLE,			   /*when dT end, i need the jackpot of combi's
						   Id*/	
	INIT_NODES,		/*cuantos nodos de cada uno hay en el modelo*/

	COUNTER_REPORT,            /*when dT end, all nodes must report your 
							status*/
	FUNCTION_REPORT,
	NORMAL_REPORT,
	COMBI_REPORT,
	QUEUE_REPORT,
	QUEUE_FINAL_REPORT,
	COUNTER_FINAL_REPORT,
	PRINT_SIGNAL, 		   /*when scheduler send to printer must write another 
						   dT story or give up it */
	COUNTER_CYCLES,    /* cuando el printer envia al scheduler los ciclos de las combis*/


	ADVANCE_PAHSE,			/* the resources only can out*/
	GENERATION_PHASE,		/* the resources only can in and procese*/
	GENERATION_PHASE_PRIMA,	/* the resources only can in but can't
							procese*/

	ADVANCE_PAHSE_PRIMA,	/* the resources only can out and tell to 
							scheduler if he terminate */

	PING_REPORT,            /*when dT end, all nodes must report your 
							status*/
	CONSUME_DT,            /*when dT end, all nodes consume one dT*/
	////------never used yet
							
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
//

/******************************************************/
//printer structs
typedef struct{
	int idNode;
	int amount;
	int counterInput;
	int counterOutput;
	double average;
	int maximun;
	int minimun;
	double timesNotEmpty;
	double percentTimesNotEmpty;
}PrinterQueue;
			
typedef struct{
	int idNode;
	int totalProductivity;
	int deltaTProductivity;
	double productivityPerTime;
}PrinterCounter;

typedef struct{
	int idNode;
	int amount;
}PrinterFunction;

typedef struct{
	int idNode;
	int activityInside;
	int counterInput;
	double delayAverage;
	int maximunDrawn;
	int minimunDrawn;
}PrinterActivity;
//contdownWorktask y delayWorktask llegan como arreglos de double en otra llamada aparte

typedef struct{
	double totalTime;
	double totalCost;
	int queuesSummaryAmount;
	int countersSummaryAmount;
}PrinterSummary;
//idNode, totalProductivity , idNode, fixCost y VariableCost llegan como arreglo en otra llamada

typedef struct{
	int idNode;
	double fixCost;
	double VariableCost;
}PrinterFinalQueue;
			
typedef struct{
	int idNode;
	int totalProductivity;
}PrinterFinalCounter;
/******************************************************/
typedef enum{
	QUEUE, NORMAL, COUNTER, FUNCTION, COMBI
}TYPENODE;


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
