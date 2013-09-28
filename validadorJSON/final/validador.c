#include "validador.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERROR                      0
#define SUCCESS                    1
#define STARTING_CAPACITY         15
#define ARRAY_MAX_CAPACITY    122880 /* 15*(2^13) */
#define OBJECT_MAX_CAPACITY      960 /* 15*(2^6)  */
#define MAX_NESTING               19
#define MAX(a, b)             ((a) > (b) ? (a) : (b))

#define parson_malloc(a)     malloc(a)
#define parson_free(a)       free((void*)a)
#define parson_realloc(a, b) realloc(a, b)

/* JSON Array API */
/*JSON_Value * json_array_get_value(const JSON_Array *array, size_t index) {
    if (index >= json_array_get_count(array)) { return NULL; }
    return array->items[index];
}

const char * json_array_get_string(const JSON_Array *array, size_t index) {
    return json_value_get_string(json_array_get_value(array, index));
}*/

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
	int sizeQueues, sizeCounters, sizeNormals, sizeFunctions, sizeCombis;
	int *arrayQueues, *arrayCounters, *arrayNormals, *arrayFunctions, *arrayCombis;
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

	/*for (i = 0; i < sizeNormals ; i++)
	    printf("%d ",arrayNormals[i]);
	printf("\n");*/

	/*DIFERENTE NUMERO DE ID*/
	if (repeatArrays(&arrayQueues, sizeQueues, &arrayCounters, sizeCounters, &arrayNormals, sizeNormals, &arrayFunctions, sizeFunctions, &arrayCombis, sizeCombis) > 0)
		return VALIDATION_FAIL; /*FAIL ID*/

	/*TODO: referencias en antecesores y predecesores existan, que no apunten a un id que no existe*/

	for (i = 0; i < sizeNormals ; i++)
	{
	    /*COLA: ANTESESOR: No pueden ser Colas.*/
		getArrayInArray(object, "transformation.queues", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(&arrayPreceders,sizePreceders,&arrayQueues,sizeQueues)> 0)
			return VALIDATION_FAIL; /*FAIL QUEUE*/
	    /*COLA: SUCESOR: Solo pueden ser combis.*/
		getArrayInArray(object, "transformation.queues", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(&arrayFollowers,sizeFollowers,&arrayCombis,sizeCombis) != sizeFollowers)
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
	}

    /*TODO: NORMAL: ANTESESOR: No pueden ser colas*/
    /*TODO: NORMAL: SUCESOR: No pueden ser COMBIS.*/
    /*TODO: FUNCION: ANTESESOR: No pueden ser colas.*/
    /*TODO: FUNCION: SUCESOR: No pueden ser Combis*/
    /*TODO: CONTADOR: ANTESESOR: No pueden ser colas ni otro contador.*/
    /*TODO: CONTADOR: SUCESOR: No pueden ser combis, ni contadores.*/

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

int repeatArrays(int **array1 ,int sizeArray1, int **array2,int sizeArray2, int **array3, int sizeArray3, int **array4, int sizeArray4, int **array5, int sizeArray5)
{
	int i=0,j,count=0;
	int sizeArray = sizeArray1 + sizeArray2 + sizeArray3 + sizeArray4 + sizeArray5;
	int *array = (int*)malloc(sizeof(int) * sizeArray);

	for (j = 0; j < sizeArray1; i++, j++)
		array[i] = (*array1)[j];
	for (j = 0; j < sizeArray2; i++, j++)
		array[i] = (*array2)[j];
	for (j = 0; j < sizeArray3; i++, j++)
		array[i] = (*array3)[j];
	for (j = 0; j < sizeArray4; i++, j++)
		array[i] = (*array4)[j];
	for (j = 0; j < sizeArray5; i++, j++)
		array[i] = (*array5)[j];

	for (i = 0; i < sizeArray ; i++)
		for (j = i+1; j < sizeArray ; j++)
	    	if (array[i] == array[j])
				count=count+1;

	return count;
}
