#include "parson.h"
#include <stddef.h>
#include <wjelement.h>    

#ifndef _MAIN_H_
#define _MAIN_H_

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

#endif


/* 
double        json_object_dotget_number (const JSON_Object *object, const char *name);
int           json_object_dotget_boolean(const JSON_Object *object, const char *name);

Functions to get available names 
size_t        json_object_get_count(const JSON_Object *object);
const char  * json_object_get_name (const JSON_Object *object, size_t index);
    
JSON_Value  * json_array_get_value  (const JSON_Array *array, size_t index);
const char  * json_array_get_string (const JSON_Array *array, size_t index);
JSON_Object * json_array_get_object (const JSON_Array *array, size_t index);
JSON_Array  * json_array_get_array  (const JSON_Array *array, size_t index);
double        json_array_get_number (const JSON_Array *array, size_t index);
int           json_array_get_boolean(const JSON_Array *array, size_t index);
size_t        json_array_get_count  (const JSON_Array *array);

JSON_Value_Type json_value_get_type   (const JSON_Value *value);
JSON_Object *   json_value_get_object (const JSON_Value *value);
JSON_Array  *   json_value_get_array  (const JSON_Value *value);
const char  *   json_value_get_string (const JSON_Value *value);
double          json_value_get_number (const JSON_Value *value);
int             json_value_get_boolean(const JSON_Value *value);*/
int					validateSchema(const char *filenameJson , const char *filenameSchema);
static void			schema_error(void *client, const char *format, ...);
static WJElement	schema_load(const char *name, void *client, const char *file, const int line);
int validateJsonImput(const char *filenameJson);
void getArray(JSON_Object *object, const char *arregloJson,const char *atributeJson ,int** arreglo, int* m);
void getArrayInArray(JSON_Object * objectJson,const char *arrayJson,int pos,const char *arrayJsonIn, int** arreglo, int* m);
int countArrayInclude(int **array,int sizeArray,int **arrayFull, int sizeArrayFull);
int repeatArrays(int **array1 ,int sizeArray1, int **array2,int sizeArray2, int **array3, int sizeArray3, int **array4, int sizeArray4, int **array5, int sizeArray5,int** array, int* sizeArray);
