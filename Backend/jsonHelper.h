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

typedef struct{
	int isValid;
	int seed;
	int watchdog;
	int* seedAndCombisId;
	int targetCounter;
	int* qCouNfComb;
	CycleValidator* targets;
	int nodesAmount;
} ValidationResults;

typedef enum{
	ERROR_OPEN_JSON =1,	/* json no existe */
	ERROR_OPEN_SCHEMA ,	/* schema no existe */
	ERROR_READ_JSON ,	/* ej: el json tenga solo una palabra */
	ERROR_READ_SCHEMA,  /* ej: el schema tenga solo una palabra */
	INVALID_MODEL_SEED,
	INVALID_QUEUE , //6
	INVALID_COMBI,
	INVALID_FUNCTION,
	INVALID_COUNTER,
	INVALID_NORMAL,
	INVALID_JSON,		/* 11 ej: sacando una coma al schema , esquema incompleto */
	INVALID_JSON_DEPTH,
	INVALID_JSON_OBJECT,
	INVALID_SCHEMA,		/* ej: sacando una coma al schema , esquema incompleto */
	DOUBLE_REFERENCE_FAIL,
	AUTOREFERENCE_FAIL, //16
	PROBABILISTIC_BRANCH_COUNT_FAIL,
	PROBABILISTIC_BRANCH_FAIL,
	VALIDATION_FAIL,	/* el json y el esquema son correctos pero el json no respeta el schema. ej: no incluir un elemento requerido o poner un number en lugar de un double*/
	VALIDATION_PASS		/* 20 validacion correcta */
}VALIDATE;

/*  //Funcion principal
Valida El archivo de ingreso contra el schema y  luego al archivo contra las reglas misma del modelo precursor */
extern ValidationResults* validateJsonInput( const char* filenameJson );
/**	Escribe el porque fracazo la validacion del json */
extern void writeErrorInFile(const char* label);
/* Valida El archivo de ingreso contra el schema */
extern int validateSchema(const char *filenameJson);
/* Handler de que hacer en caso de que falle la validación */
extern void schema_error(void *client, const char *format, ...);
extern WJElement schema_load(const char *name, void *client, const char *file, const int line);
extern ValidationResults*  validateJson(const char *filenameJson);
extern int getIdNodeArray( JSON_Object *jsonObject, const char *arrayName, int** array );
extern int repeatArrays( const int *const queues ,const int queueSize,
				  const int *const counters, const int counterSize, 
				  const int * const normals, const int normalSize, 
				  const int * const functions, const int functionSize, 
				  const int *const combis, const int combiSize, 
				  int** allNodes, int* sizeAllNodes);
extern void MergeSort(int firstElement, int lastElement, int** array);
extern void Merge(int firstElement, int middleElement, int lastElement, int** array);
/* Obtiene la lista links (arrayJsonIn= followers o proceders)  de cada nodo (nodeName) y lo pone en una pocicion de una tabla segun el id "idNode" */
extern void getLinkTable(int** linkTable, const char *nodeName, const char *arrayJsonIn, JSON_Object *object );
extern int validateDoubleReference(const int sizeAllNodes, int** precederArrayFull,int** followerArrayFull);
/* cada link de unoo a otro es unico(no hace falta validar el follower)*/
extern int validateEachLinkIsUnique(const int sizeAllNodes, int** precederArrayFull);
/* Valida que un nodo no se apunte a si mismo en las referencias */
extern int validateAutoreference(const int sizeAllNodes, int** precederArrayFull);
/* Valida que las semillas del modelo estan en el rango adecuado */
extern int validateSeeds(JSON_Object* object);
/*El nodo X no tiene referencias */
extern int disjointSet(const int * const linkBag, const int linkBagSize, const int *const singleTypeNodeBag, const int nodeBagSize);
/*El nodo X solo tiene referencias justas*/
extern int jointSet(const int * const linkBag, const int linkBagSize, const int *const singleTypeNodeBag, const int nodeBagSize);
/*Obtiene la lista de probabilidad de cada rama de un nodo */   
extern void getProbabilisticBranch(JSON_Object * objectJson,const char *arrayNodeName, const int pos,const char *arrayName,
extern double** probabilisticBranchArray, int* probabilisticBranchSize);
/* Valida la probabilidad de cada rama y todas en conjunto*/
extern int validateProbabilisticBranch(const double* probabilisticBranch, const int probabilisticCounter, const int  followCounter);
/* obtiene la cantidad de ciclos que deberia cumplir cada counter */
extern CycleValidator* getTargets( JSON_Object* object);
