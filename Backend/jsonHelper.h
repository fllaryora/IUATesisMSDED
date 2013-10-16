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

extern int freeAllAndReturn(int * , int* , int * , int * , 	int * ,int * ,int * ,int * , JSON_Value  * , const int );
extern void	schema_error(void *client, const char *format, ...);
extern WJElement	schema_load(const char *name, void *client, const char *file, const int line);

extern int validateJsonInput(const char *,const char *);

extern int validateSchema(const char *,const char *);
extern int validateJson(const char *);

extern void getArray(JSON_Object *, const char *,const char *,int** , int* );
extern void getArrayInArray(JSON_Object *,const char *,int ,const char *, int** , int* );
extern int countArrayInclude(const int * const , const int , const int *const , const int );
extern int repeatArrays(const int *const  ,const int , const int * const , const int , const int * const , const int , const int * const , const int , const int *const , const int , int** , int* );
extern int getNodesAmount( const char * );
extern int* getCombiIds( const char * );
extern int* getTargets( const char *);
extern int getWatchdog( const char *);

#endif /* #ifndef _JSON_HELPER_H_*/
