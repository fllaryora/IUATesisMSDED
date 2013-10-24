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

void printQueue(Queue queue);
void printCombi(Combi combi);
int getCountNodes(int **arrayCountNodes);
void getQueues(Queue **queues, int *queuesCount);
void getCounters(Counter **counters, int *counterCount);
void getFunctions(Function **functions, int *functionCount);
void getNormals(Normal **normals, int *normalCount);
void getCombis(Combi **combis, int *combiCount);

#endif /* #ifndef _MAIN_H_*/


