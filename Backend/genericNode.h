#ifndef _GENERIC_NODE_H_
#define _GENERIC_NODE_H_
#include "parson.h"
#include "jsonHelper.h"

extern void genericNode(int myIdNodo);

typedef enum{
	DIST_UNIFORM,
	DIST_DETERMINISTIC,
	DIST_NORMAL,
	DIST_EXPONENTIAL,
	DIST_TRIANGULAR,
	DIST_BETA,
	DIST_LOG_NORMAL
}DISTRIBUTION;

typedef struct{
	int distribution;
	double least;
	double highest;
	double constant;
	double mean;
	double variance;
	double lambda;
	double mode;
	double minimun;
	double maximun;
	double shapeAlpha;
	double shapeBeta;
	double shape;
	double escale;
	int seed;
}Delay;

typedef struct{
    int idNode;
    int resource;
    int countPreceders;
    int *preceders;
    int countFollowers;
    int *followers;
    double fixedCost;
    double variableCost;
}Queue;

typedef struct{
    int idNode;
    int quantity;
    double cycle;
    int countPreceders;
    int *preceders;
    int countFollowers;
    int *followers;
}Counter;

typedef struct{
	int idNode;
    int input;
	int output;
    int countPreceders;
    int *preceders;
    int countFollowers;
    int *followers;
    int countProbabilisticBranch;
    double *probabilisticBranch;
}Function;

typedef struct{
    int idNode;
    int countPreceders;
    int *preceders;
    int countFollowers;
	int *followers;
    int countProbabilisticBranch;
    double *probabilisticBranch;
	Delay delay;
}Normal;

typedef struct{
	int idNode;
    int countPreceders;
    int *preceders;
    int countFollowers;
	int *followers;
    int countProbabilisticBranch;
    double *probabilisticBranch;
	Delay delay;
}Combi;

extern void receiveQueue(Queue *queue);
extern void receiveCounter(Counter *counter);
extern void receiveFunction(Function *function);
extern void receiveNormal(Normal *normal);
extern void receiveCombi(Combi *combis);

extern void sendStructToNodes( const char * );
extern void sendStruct(Queue **, int *,Counter **, int *,Function **, int *,Normal **, int *,Combi **, int *);

extern void getQueues(const char * , Queue **, int *);
extern void getCounters(const char * , Counter **, int *);
extern void getFunctions(const char * , Function **, int *);
extern void getNormals(const char * , Normal **, int *);
extern void getCombis(const char * , Combi **, int *);

extern void printQueue(Queue );
extern void printCombi(Combi );
extern void printCounter(Counter );

extern void sendStruct(Queue **queues,		  int *queuesCount,
				Counter **counters,	  int *counterCount,
				Function **functions, int *functionCount,
				Normal **normals,	  int *normalCount,
				Combi **combis, 	  int *combiCount);
#endif /* #ifndef _GENERIC_NODE_H_*/
