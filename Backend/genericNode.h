#ifndef _GENERIC_NODE_H_
#define _GENERIC_NODE_H_
#include "parson.h"
#include "jsonHelper.h"

#define TIME_TO_DELTA_T 1//10000 //admitimos hasta 4 ceros en los doubles
extern void genericNode(const int , const int ,const MPI_Comm, const int );

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


typedef struct WorkTaskType{
        unsigned long long int  initialDelay;
        unsigned long long int  currentDelay;
        struct WorkTaskType *next;
}Worktask;



extern void receiveQueue(const MPI_Comm ,Queue *queue);
extern void receiveCounter(const MPI_Comm ,Counter *counter);
extern void receiveFunction(const MPI_Comm ,Function *function);
extern void receiveNormal(const MPI_Comm ,Normal *normal);
extern void receiveCombi(const MPI_Comm ,Combi *combis);

extern void sendStructToNodes( const char * ,const MPI_Comm commNodes);
extern void sendStruct( const MPI_Comm commNodes , Queue *,const int ,  Counter *,const  int , Function *, const int , Normal *, const int , Combi *, const int );

extern void getQueues(const char * , Queue **, int *);
extern void getCounters(const char * , Counter **, int *);
extern void getFunctions(const char * , Function **, int *);
extern void getNormals(const char * , Normal **, int *);
extern void getCombis(const char * , Combi **, int *);

extern void printQueue(Queue );
extern void printCombi(Combi );
extern void printCounter(Counter );
extern void printFunction(Function );
extern void printNormal(Normal );


void insertWorktask(Worktask *pointer, unsigned long long int delay);
int discountDelayAndDeleteFinishedWorktask(Worktask *pointer);
#endif /* #ifndef _GENERIC_NODE_H_*/

