#ifndef _JSON_HELPER_H_
#define _JSON_HELPER_H_

#include "parson.h"
#include "ourMPI.h"
#include "main.h"
#include <stddef.h>
#include <wjelement.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROB_MIN 0.999
#define PROB_MAX 1.001

typedef enum{
	ERROR_OPEN_JSON =1,	/* json no existe */
	ERROR_OPEN_SCHEMA ,	/* schema no existe */
	ERROR_READ_JSON ,	/* ej: el json tenga solo una palabra */
	ERROR_READ_SCHEMA,  /* ej: el schema tenga solo una palabra */
	INVALID_QUEUE , //5
	INVALID_COMBI,
	INVALID_FUNCTION,
	INVALID_COUNTER,
	INVALID_NORMAL,
	INVALID_JSON,		/* 10 ej: sacando una coma al schema , esquema incompleto */
	INVALID_JSON_DEPTH,
	INVALID_JSON_OBJECT,
	INVALID_SCHEMA,		/* ej: sacando una coma al schema , esquema incompleto */
	DOUBLE_REFERENCE_FAIL,
	AUTOREFERENCE_FAIL, //15
	PROBABILISTIC_BRANCH_COUNT_FAIL,
	PROBABILISTIC_BRANCH_FAIL,
	VALIDATION_FAIL,	/* el json y el esquema son correctos pero el json no respeta el schema. ej: no incluir un elemento requerido o poner un number en lugar de un double*/
	VALIDATION_PASS		/* 19 validacion correcta */
}VALIDATE;

extern void MergeSort(int , int , int** );
extern void Merge(int , int , int , int** );

extern int freeAllAndReturn(int * , int* , int * , int * , 	int * ,int * ,int * ,int * , JSON_Value  * , const int );
extern void	schema_error(void *client, const char *format, ...);
extern WJElement	schema_load(const char *name, void *client, const char *file, const int line);

extern int validateJsonInput(const char *);

extern int validateSchema(const char *);
extern int validateJson(const char *);

extern void getArray(JSON_Object *, const char *,const char *,int** , int* );
extern void getArrayInArray(JSON_Object *,const char *,int ,const char *, int** , int* );
extern int countArrayInclude(const int * const , const int , const int *const , const int );
extern int repeatArrays(const int *const  ,const int , const int * const , const int , const int * const , const int , const int * const , const int , const int *const , const int , int** , int* );
extern int getNodesAmount( const char * );
extern int* getCombiIds( const char * );
extern int* getTargets( const char *);
extern int getWatchdog( const char *);
extern int getModelSeed( const char *);
extern void getArrayBidimencionalFull(JSON_Object *,int*** ,const char *,int i, int** , int* );
extern int validateDoubleReference(int ,int*** ,int*** );
extern int validateAutoreference(JSON_Object *, const char *,int ,int** , int* );
extern int validateProbabilisticBranch(double** , int* , int* );
extern void getArrayInArrayDouble(JSON_Object * ,const char *,int ,const char *, double** , int* );
#endif /* #ifndef _JSON_HELPER_H_*/
