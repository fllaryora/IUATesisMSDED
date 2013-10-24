#ifndef _JSON_HELPER_H_
#define _JSON_HELPER_H_

#include "parson.h"
#include <stddef.h>
#include <wjelement.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum{
	ERROR_OPEN_JSON,	/* json no existe */
	ERROR_OPEN_SCHEMA,	/* schema no existe */
	ERROR_READ_JSON,	/* ej: el json tenga solo una palabra */
	ERROR_READ_SCHEMA,  /* ej: el schema tenga solo una palabra */
	INVALID_JSON,		/* ej: sacando una coma al schema , esquema incompleto */
	INVALID_SCHEMA,		/* ej: sacando una coma al schema , esquema incompleto */
	VALIDATION_FAIL,	/* el json y el esquema son correctos pero el json no respeta el schema. ej: no incluir un elemento requerido o poner un number en lugar de un double*/
	VALIDATION_PASS		/* validacion correcta */
}VALIDATE;

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

/*	Delay
	char *distribution; /*uniform/
	double least
	double highest
	int seed

	char *distribution; /*deterministic/
	double constant;

	char *distribution; /*normal/
	double mean;
	double variance;
	int seed

	char *distribution; /*exponential/
	double lambda;
	int seed

	char *distribution; /*triangular/
	double least
	double highest
	double mode
	integer seed

	char *distribution; /*beta/
	double minimun;
	double maximun;
	double shapeAlpha;
	double shapeBeta
	int seed;

	char *distribution; /*log-normal/
	double escale
	double shape
	double minimun
	int seed
*/

extern static void	schema_error(void *client, const char *format, ...);
extern static WJElement	schema_load(const char *name, void *client, const char *file, const int line);

extern int validateJsonInput(const char *, const char *));

extern int validateSchema(const char *, const char *);
extern int validateJson(const char *);

extern void getArray(JSON_Object *, const char *,const char *,int** , int* );
extern void getArrayInArray(JSON_Object *,const char *,int ,const char *, int** , int* );
extern int countArrayInclude(int **, int, int **, int );
extern int repeatArrays(int ** , int, int **, int, int **, int, int **, int, int **, int ,int**, int*);

extern void sendStructToNodes( const char * );
extern void sendStruct(Queue **, int *,Counter **, int *,Function **, int *,Normal **, int *,Combi **, int *);

extern void getQueues(const char * , Queue **, int *);
extern void getCounters(const char * , Counter **, int *);
extern void getFunctions(const char * , Function **, int *);
extern void getNormals(const char * , Normal **, int *);
extern void getCombis(const char * , Combi **, int *);

extern void printQueue(Queue );
extern void printCombi(Combi );

extern int getNodesAmount( void );
extern int* getCombiIds( void );

extern void sendStruct(Queue **queues,		  int *queuesCount,
				Counter **counters,	  int *counterCount,
				Function **functions, int *functionCount,
				Normal **normals,	  int *normalCount,
				Combi **combis, 	  int *combiCount);

#endif /* #ifndef _JSON_HELPER_H_*/
