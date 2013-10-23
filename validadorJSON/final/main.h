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

extern void printQueue(Queue);
void printCombi(Combi combi);

#endif /* #ifndef _MAIN_H_*/


