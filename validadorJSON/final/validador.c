#include "validador.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERROR                      0
#define SUCCESS                    1

int validateSchema(const char *filenameJson , const char *filenameSchema)
{
	FILE *jsonfile;
	FILE *schemafile;
	WJReader readjson;
	WJReader readschema;
	WJElement json;
	WJElement schema;
	XplBool succ;
	char *format=NULL;

	if(!(jsonfile = fopen(filenameJson, "r"))) {
		return ERROR_OPEN_JSON;
	}
	if(!(schemafile = fopen(filenameSchema, "r"))) {
		return ERROR_OPEN_SCHEMA;
	}

	if(!(readjson = WJROpenFILEDocument(jsonfile, NULL, 0)) || !(json = WJEOpenDocument(readjson, NULL, NULL, NULL))) {
		return ERROR_READ_JSON;
	}
	if(!(readschema = WJROpenFILEDocument(schemafile, NULL, 0)) || !(schema = WJEOpenDocument(readschema, NULL, NULL, NULL))) {
		WJECloseDocument(json);
		return ERROR_READ_SCHEMA;
	}

	/*WJEDump(json);*/
	if (readjson->depth)
	{
		return INVALID_JSON;
	}

	/*WJEDump(schema);*/
	if (readschema->depth)
	{
		return INVALID_SCHEMA;
	}

	if(!WJESchemaValidate(schema, json, schema_error, schema_load, NULL, format)) {
		WJECloseDocument(json);
		WJECloseDocument(schema);
		WJRCloseDocument(readjson);
		WJRCloseDocument(readschema);
		fclose(jsonfile);
		fclose(schemafile);
		return VALIDATION_FAIL;
	}

	WJECloseDocument(json);
	WJECloseDocument(schema);
	WJRCloseDocument(readjson);
	WJRCloseDocument(readschema);
	fclose(jsonfile);
	fclose(schemafile);
	return VALIDATION_PASS;
}

static void schema_error(void *client, const char *format, ...)
{
	return;
}

static WJElement schema_load(const char *name, void *client, const char *file, const int line)
{
	char *format;
	char *path;
	FILE *schemafile;
	WJReader readschema;
	WJElement schema;

	schema = NULL;
	if(client && name) {
		format = (char *)client;
		path = malloc(strlen(format) + strlen(name));
		sprintf(path, format, name);

		if(schemafile = fopen(path, "r")) {
			if((readschema = WJROpenFILEDocument(schemafile, NULL, 0))) {
				schema = WJEOpenDocument(readschema, NULL, NULL, NULL);
			} /*else {
				fprintf(stderr, "json document failed to open: '%s'\n", path);
			}*/
			fclose(schemafile);
		} /*else {
			fprintf(stderr, "json file not found: '%s'\n", path);
		}*/
		free(path);
	}
	return schema;
}

int validateJsonImput(const char *filenameJson)
{
    JSON_Value  *root_value;
    JSON_Object *object,*objectInArray;
    JSON_Array  *array;
	int i, j;
	int sizeQueues, sizeCounters, sizeNormals, sizeFunctions, sizeCombis, sizeNodes;
	int *arrayQueues, *arrayCounters, *arrayNormals, *arrayFunctions, *arrayCombis,*arrayNodes;
	int sizePreceders, sizeFollowers;
	int *arrayPreceders, *arrayFollowers;

    /*validate json*/
    root_value = json_parse_file(filenameJson);
    if (!root_value)
    {
		return INVALID_JSON;
    }

    if (json_value_get_type(root_value) == JSONArray)
    {
		return INVALID_JSON;
    }

    if (json_value_get_type(root_value) != JSONObject)
    {
		return INVALID_JSON;
    }

    object = json_value_get_object(root_value);

	getArray(object, "transformation.queues"   , "idNode" ,&arrayQueues    , &sizeQueues);
	getArray(object, "transformation.counters" , "idNode" ,&arrayCounters  , &sizeCounters);
	getArray(object, "transformation.normals"  , "idNode" ,&arrayNormals   , &sizeNormals);
	getArray(object, "transformation.functions", "idNode" ,&arrayFunctions , &sizeFunctions);
	getArray(object, "transformation.combis"   , "idNode" ,&arrayCombis    , &sizeCombis);
	/*for (i = 0; i < sizeNormals ; i++) printf("%d ",arrayNormals[i]); printf("\n");*/

	/*DIFERENTE NUMERO DE ID*/
	if (repeatArrays(&arrayQueues, sizeQueues, &arrayCounters, sizeCounters, &arrayNormals, sizeNormals, &arrayFunctions, sizeFunctions, &arrayCombis, sizeCombis,&arrayNodes,&sizeNodes) > 0)
		return VALIDATION_FAIL; /*FAIL ID*/

	for (i = 0; i < sizeQueues ; i++)
	{
	    /*COLA: ANTESESOR: No pueden ser Colas.*/
		getArrayInArray(object, "transformation.queues", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayQueues,sizeQueues)> 0)
			return VALIDATION_FAIL; /*FAIL QUEUE*/
	    /*COLA: SUCESOR: Solo pueden ser combis.*/
		getArrayInArray(object, "transformation.queues", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayCombis,sizeCombis) != sizeFollowers)
			return VALIDATION_FAIL; /*FAIL QUEUE*/
		/*COLA: Existe referencia Preceders Followers*/
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayNodes,sizeNodes)!= sizePreceders)
			return VALIDATION_FAIL; /*FAIL QUEUE*/
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayNodes,sizeNodes)!= sizeFollowers)
			return VALIDATION_FAIL; /*FAIL QUEUE*/
	}

	for (i = 0; i < sizeCombis ; i++)
	{
	    /*COMBI: ANTESESOR: Solo pueden ser nodos Cola.*/
		getArrayInArray(object, "transformation.combis", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayQueues,sizeQueues) !=  sizePreceders)
			return VALIDATION_FAIL; /*FAIL COMBI*/
    	/*COMBI: SUCESOR: No pueden ser Combis.*/
		getArrayInArray(object, "transformation.combis", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayCombis,sizeCombis) >0)
			return VALIDATION_FAIL; /*FAIL COMBI*/
		/*COMBI: Existe referencia Preceders Followers*/
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayNodes,sizeNodes)!= sizePreceders)
			return VALIDATION_FAIL; /*FAIL COMBI*/
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayNodes,sizeNodes)!= sizeFollowers)
			return VALIDATION_FAIL; /*FAIL COMBI*/
	}

	for (i = 0; i < sizeNormals ; i++)
	{
		/*NORMAL: ANTESESOR: No pueden ser colas*/
		getArrayInArray(object, "transformation.normals", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayQueues,sizeQueues)> 0)
			return VALIDATION_FAIL; /*FAIL NORMAL*/
    	/*NORMAL: SUCESOR: No pueden ser combis.*/
		getArrayInArray(object, "transformation.normals", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayCombis,sizeCombis) >0)
			return VALIDATION_FAIL; /*FAIL NORMAL*/
		/*NORMAL: Existe referencia Preceders Followers*/
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayNodes,sizeNodes)!= sizePreceders)
			return VALIDATION_FAIL; /*FAIL NORMAL*/
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayNodes,sizeNodes)!= sizeFollowers)
			return VALIDATION_FAIL; /*FAIL NORMAL*/
	}

	for (i = 0; i < sizeFunctions ; i++)
	{
		/*FUNCION: ANTESESOR: No pueden ser colas*/
		getArrayInArray(object, "transformation.functions", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayQueues,sizeQueues)> 0)
			return VALIDATION_FAIL; /*FAIL FUNCTION*/
    	/*FUNCION: SUCESOR: No pueden ser combis.*/
		getArrayInArray(object, "transformation.functions", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayCombis,sizeCombis) >0)
			return VALIDATION_FAIL; /*FAIL FUNCTION*/
		/*FUNCION: Existe referencia Preceders Followers*/
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayNodes,sizeNodes)!= sizePreceders)
			return VALIDATION_FAIL; /*FAIL FUNCION*/
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayNodes,sizeNodes)!= sizeFollowers)
			return VALIDATION_FAIL; /*FAIL FUNCION*/
	}

	for (i = 0; i < sizeCounters ; i++)
	{
    	/*CONTADOR: ANTESESOR: No pueden ser colas ni otro contador.*/
		getArrayInArray(object, "transformation.counters", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayQueues,sizeQueues)> 0 || countArrayInclude(&arrayPreceders,sizePreceders,&arrayCounters,sizeCounters)> 0)
			return VALIDATION_FAIL; /*FAIL CONTADOR*/
    	/*CONTADOR: SUCESOR: No pueden ser combis, ni contadores.*/
		getArrayInArray(object, "transformation.counters", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayCombis,sizeCombis) >0 || countArrayInclude(&arrayFollowers,sizeFollowers,&arrayCounters,sizeCounters)> 0)
			return VALIDATION_FAIL; /*FAIL CONTADOR*/
		/*CONTADOR: Existe referencia Preceders Followers*/
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayNodes,sizeNodes)!= sizePreceders)
			return VALIDATION_FAIL; /*FAIL CONTADOR*/
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayNodes,sizeNodes)!= sizeFollowers)
			return VALIDATION_FAIL; /*FAIL CONTADOR*/
	}

    json_value_free(root_value);
    return VALIDATION_PASS;
}

void getArray(JSON_Object *objectJson, const char *arrayJson, const char *atributeJson, int** array, int* sizeArray)
{
	int i;
	JSON_Array *arrayJsonFunction = json_object_dotget_array(objectJson, arrayJson);
    JSON_Object *objectInArray;
    *sizeArray = json_array_get_count(arrayJsonFunction);
    *array = (int*)malloc(sizeof(int) * json_array_get_count(arrayJsonFunction));

    for (i = 0; i < json_array_get_count(arrayJsonFunction); i++)
	{
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    (*array)[i] = json_object_dotget_number(objectInArray, atributeJson );
	}

}

void getArrayInArray(JSON_Object * objectJson,const char *arrayJson,int pos,const char *arrayJsonIn, int** arreglo, int* m)
{
	JSON_Array *array = json_object_dotget_array(objectJson, arrayJson);
	JSON_Object *objectInArray = json_array_get_object(array, pos);
	JSON_Array *arrayJsonFunction = json_object_dotget_array(objectInArray, arrayJsonIn);
	int i;
    *m = json_array_get_count(arrayJsonFunction);
    *arreglo = (int*)malloc(sizeof(int) * json_array_get_count(arrayJsonFunction));

    for (i = 0; i < json_array_get_count(arrayJsonFunction); i++)
	{
	    (*arreglo)[i] = json_array_get_number(arrayJsonFunction, i);
	}
}

int countArrayInclude(int **array,int sizeArray,int **arrayFull, int sizeArrayFull)
{
	int i,j, count=0;
    for (i = 0; i < sizeArray; i++)
	{
		for (j = 0; j < sizeArrayFull; j++)
		{
			if ((*array)[i] == (*arrayFull)[j])
			{
				count = count+1;
				break;
			}
		}
	}
	return count;
}

int repeatArrays(int **array1 ,int sizeArray1, int **array2,int sizeArray2, int **array3, int sizeArray3, int **array4, int sizeArray4, int **array5, int sizeArray5, int** array, int* sizeArray)
{
	int i=0,j,count=0;
	*sizeArray = sizeArray1 + sizeArray2 + sizeArray3 + sizeArray4 + sizeArray5;
	*array = (int*)malloc(sizeof(int) * (*sizeArray));

	for (j = 0; j < sizeArray1; i++, j++)
		(*array)[i] = (*array1)[j];
	for (j = 0; j < sizeArray2; i++, j++)
		(*array)[i] = (*array2)[j];
	for (j = 0; j < sizeArray3; i++, j++)
		(*array)[i] = (*array3)[j];
	for (j = 0; j < sizeArray4; i++, j++)
		(*array)[i] = (*array4)[j];
	for (j = 0; j < sizeArray5; i++, j++)
		(*array)[i] = (*array5)[j];

	for (i = 0; i < *sizeArray ; i++)
		for (j = i+1; j < *sizeArray ; j++)
	    	if ((*array)[i] == (*array)[j])
				count=count+1;

	return count;
}

/* MANEJO DE ESTRUCTURA */

void getQueues(const char *filenameJson , Queue **queues, int *queuesCount)
{
	*queuesCount = 2;

	*queues = (Queue *) malloc(2*sizeof(Queue));
	(*queues)[0].idNode = 44;
   	(*queues)[0].resource = 2;
	(*queues)[0].fixedCost = 9.8;
	(*queues)[0].variableCost = 7.5;
	(*queues)[0].countPreceders = 2;
	(*queues)[0].preceders = (int *) malloc(2*sizeof(int));
	(*queues)[0].preceders[0]=2;
	(*queues)[0].preceders[1]=3;
	(*queues)[0].countFollowers = 3;
	(*queues)[0].followers = (int *) malloc(3*sizeof(int));
	(*queues)[0].followers[0]=7;
	(*queues)[0].followers[1]=10;
	(*queues)[0].followers[2]=12;

	(*queues)[1].idNode = 2;
   	(*queues)[1].resource = 0;
	(*queues)[1].fixedCost = 0.8;
	(*queues)[1].variableCost = 0.5;
	(*queues)[1].countPreceders = 1;
	(*queues)[1].preceders = (int *) malloc(1*sizeof(int));
	(*queues)[1].preceders[0]=99;
	(*queues)[1].countFollowers = 1;
	(*queues)[1].followers = (int *) malloc(1*sizeof(int));
	(*queues)[1].followers[0]=6;

}

void getCounters(const char *filenameJson , Counter **normals, int *counterCount)
{
	*counterCount = 0;
}

void getFunctions(const char *filenameJson , Function **functions, int *functionCount)
{
	*functionCount = 0;
}

void getNormals(const char *filenameJson , Normal **normals, int *normalCount)
{
	*normalCount = 0;
}

void getCombis(const char *filenameJson , Combi **combis, int *combiCount)
{
	*combiCount = 1;

	*combis = (Combi *) malloc(2*sizeof(Combi));
	(*combis)[0].idNode = 86;
	(*combis)[0].countPreceders = 2;
	(*combis)[0].preceders = (int *) malloc(2*sizeof(int));
	(*combis)[0].preceders[0] = 2;
	(*combis)[0].preceders[1] = 44;
	(*combis)[0].countFollowers = 3;
	(*combis)[0].followers = (int *) malloc(3*sizeof(int));
	(*combis)[0].followers[0] = 101;
	(*combis)[0].followers[1] = 102;
	(*combis)[0].followers[2] = 105;
	(*combis)[0].countProbabilisticBranch = 3;
	(*combis)[0].probabilisticBranch = (double *) malloc(3*sizeof(double));
	(*combis)[0].probabilisticBranch[0] = 25.5;
	(*combis)[0].probabilisticBranch[1] = 24.5;
	(*combis)[0].probabilisticBranch[2] = 50;
	(*combis)[0].delay.distribution = DIST_UNIFORM; //uniform
	(*combis)[0].delay.least = 1.56;
	(*combis)[0].delay.highest =  8.23;
	(*combis)[0].delay.seed = 895;
}

void printQueue(Queue queue)
{
	int i;

	printf("idNode: %d\n", queue.idNode);
	printf("resource: %d\n", queue.resource);
	printf("fixedCost: %.4f\n", queue.fixedCost);
	printf("variableCost: %.4f\n", queue.variableCost);
	printf("countPreceders: %d\n", queue.countPreceders);
	printf("countFollowers: %d\n", queue.countFollowers);

	for (i=0 ; i<queue.countPreceders ; i++)
		printf("preceders[%d]: %d\n", i,queue.preceders[i]);

	for (i=0 ; i<queue.countFollowers ; i++)
		printf("followers[%d]: %d\n", i,queue.followers[i]);
}

void printCombi(Combi combi)
{
	int i;

	printf("idNode: %d\n", combi.idNode);
	printf("countPreceders: %d\n", combi.countPreceders);
	printf("countFollowers: %d\n", combi.countFollowers);
	printf("countProbabilisticBranch: %d\n", combi.countProbabilisticBranch);
	printf("delay.distribution: %d\n",combi.delay.distribution);
	printf("delay.least: %.4f\n",combi.delay.least);
	printf("delay.highest: %.4f\n",combi.delay.highest);
	printf("delay.seed: %d\n",combi.delay.seed);

	for (i=0 ; i<combi.countPreceders ; i++)
		printf("preceders[%d]: %d\n", i,combi.preceders[i]);

	for (i=0 ; i<combi.countFollowers ; i++)
		printf("followers[%d]: %d\n", i,combi.followers[i]);

	for (i=0 ; i<combi.countProbabilisticBranch ; i++)
		printf("probabilisticBranch[%d]: %.2f\n", i,combi.probabilisticBranch[i]);
}
