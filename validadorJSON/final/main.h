#ifndef _MAIN_H_
#define _MAIN_H_

#define MASTER_ID	0
#define RAFFLER_ID	1
#define PRINTER_ID	2
#define FIST_NODE_ID	3
#define	RAFFLER_PRINTER	2
#define MASTER_RAFFLER_PRINTER	3

typedef enum{
	CONS1,
	CONS2
}OTRO;

typedef enum{
	QUEUE, NORMAL, COUNTER, FUNCTION, COMBI
}TYPENODE;

void sendStruct(Queue **queues,		  int *queuesCount,
				Counter **counters,	  int *counterCount,
				Function **functions, int *functionCount,
				Normal **normals,	  int *normalCount,
				Combi **combis, 	  int *combiCount);

void receiveQueue(Queue *queue);
void receiveCounter(Counter *counter);
void receiveFunction(Function *function);
void receiveNormal(Normal *normal);
void receiveCombi(Combi *combis);

#endif /* #ifndef _MAIN_H_*/

