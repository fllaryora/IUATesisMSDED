#include "parson.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define FIRST_KEY "LENGTH"
#define SECOND_KEY "CYCLES"
#define THRID_KEY "transformacion"
#define getType(X, Y)	json_value_get_type(json_object_dotget_value  ( X , Y))

#define N1_KEY	"colas"
#define N2_KEY	"contadores"
#define N3_KEY	"funciones"
#define N4_KEY	"normales"
#define N5_KEY	"combis"

#define Q1_KEY	"idnodo"
#define Q2_KEY	"recursos"
#define Q3_KEY	"Antecesor"
#define Q4_KEY	"Sucesor"
#define Q5_KEY	"costoFijo"
#define Q6_KEY	"costoVariable"

int validateSchema(JSON_Value* root_value);
int validateNodesTypes(const JSON_Object* nodes);
int rockNrollNENE ( const JSON_Object* object, size_t elementExpected, char** keyExpected, JSON_Value_Type* typeExpected);

int validateQueues(const JSON_Array* queues);
int validateCombis(const JSON_Array* combis);
int validateCounters(const JSON_Array* counters);
int validateNormals(const JSON_Array* normals);
int validateFunctions(const JSON_Array* functions);

int main() {

    const char *filename = "tests/modelo.txt";
    JSON_Value  *root_value;
    root_value = json_parse_file(filename);
    if ( validateSchema(root_value) == FALSE ){
		json_value_free(root_value);
		return 1;
	}
	json_value_free(root_value);
    return 0;
}

int validateSchema(JSON_Value* root_value){
	char* keyExpected[3] = {FIRST_KEY, SECOND_KEY, THRID_KEY};
	JSON_Value_Type typeExpected[3] = {JSONNumber, JSONNumber, JSONObject};
	const JSON_Object* object;
	if (root_value == NULL ){
		printf("El archivo no tiene formato Json.\n");
		return FALSE;
    }
	if (json_value_get_type(root_value) != JSONObject){
		printf("La raíz del archivo no contiene un JsonObject.\n");
		return FALSE;
    }
	object = json_value_get_object(root_value);
    if( rockNrollNENE( object, 3, keyExpected, typeExpected) == FALSE ){
		return FALSE;
	}
    return validateNodesTypes( json_object_get_object (object, THRID_KEY));
}

int validateNodesTypes(const JSON_Object* nodes){
	char* keyExpected[5] = {N1_KEY, N2_KEY, N3_KEY , N4_KEY, N5_KEY};
	int currentFunction;
	JSON_Value_Type typeExpected[5] = {JSONArray,JSONArray,JSONArray,JSONArray,JSONArray};
	int (*ptrFunctions[5]) (const JSON_Array* x) = {validateQueues, validateCombis, validateFunctions, validateNormals, validateCounters};
	rockNrollNENE(nodes, 5, keyExpected, typeExpected);
	
	for(currentFunction = 0; currentFunction < 5;currentFunction++){
		if( (*ptrFunctions[currentFunction])( json_object_get_array  (nodes, keyExpected[currentFunction]) ) == FALSE){
			return FALSE;
		}
	}
	printf("Todo OK.\n");
    return TRUE;
}

int rockNrollNENE(const JSON_Object* object, size_t elementExpected, 	char** keyExpected, JSON_Value_Type* typeExpected){
	size_t elementsInNodes, currentElement;
	const char* currentKey;
	
	elementsInNodes = json_object_get_count(object);
	if (elementsInNodes != elementExpected){
		printf("Se esperaban %d claves hijas(hijos != %d).\n", (int)elementExpected, (int)elementExpected);
		return FALSE;
    }
    for (currentElement = 0 ; currentElement < elementsInNodes ; currentElement++){
		currentKey = json_object_get_name (object, currentElement);
		if ( currentKey == NULL || strcmp( currentKey , keyExpected[currentElement]) != 0 ){
			printf("Se esperaba %s como %dº clave.\n", keyExpected[currentElement] , ((int)currentElement)+1);
			return FALSE;
		}
		
		if(getType( object, currentKey) != typeExpected[currentElement]){
			printf("Se esperaba el tipo nro %d el la  %dº  clave.\n", (int)typeExpected[currentElement] ,((int)currentElement)+1);
			return FALSE;
		}
    }
	return TRUE;
}

int validateQueues(const JSON_Array* queues){
	size_t  queuesNodesAmount, currentElement;
	const JSON_Object* currentObject;
	queuesNodesAmount = json_array_get_count  (queues);
	printf("%d nodos cola.\n",  ((int)queuesNodesAmount));
	for(currentElement = 0 ; currentElement < queuesNodesAmount ; currentElement++){
		currentObject = json_array_get_object (queues, currentElement);
		//rockNrollNENE(currentObject, 6, char** keyExpected, JSON_Value_Type* typeExpected)
	}
	
	return TRUE;
}
int validateCombis(const JSON_Array* combis){
	size_t  queuesNodesAmount, currentElement;
	const JSON_Object* currentObject;
	queuesNodesAmount = json_array_get_count  (combis);
	printf("%d nodos cola.\n",  ((int)queuesNodesAmount));
	return TRUE;
}
int validateCounters(const JSON_Array* counters){
	size_t  queuesNodesAmount, currentElement;
	const JSON_Object* currentObject;
	queuesNodesAmount = json_array_get_count  (counters);
	printf("%d nodos cola.\n",  ((int)queuesNodesAmount));
	return TRUE;
}
int validateNormals(const JSON_Array* normals){
	size_t  queuesNodesAmount, currentElement;
	const JSON_Object* currentObject;
	queuesNodesAmount = json_array_get_count  (normals);
	printf("%d nodos cola.\n",  ((int)queuesNodesAmount));
	return TRUE;
}
int validateFunctions(const JSON_Array* functions){
	size_t  queuesNodesAmount, currentElement;
	const JSON_Object* currentObject;
	queuesNodesAmount = json_array_get_count  (functions);
	printf("%d nodos cola.\n",  ((int)queuesNodesAmount));
	return TRUE;
}
