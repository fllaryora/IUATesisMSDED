#include "jsonHelper.h"

/* 
limpia memoria y retorna un valor
*/
int freeAllAndReturn(int *arrayQueues , int*arrayCounters , int *arrayNormals , int *arrayFunctions , 
	int *arrayCombis ,int *arrayNodes ,int *arrayPreceders ,int *arrayFollowers , JSON_Value  *root_value , const int ret){
	free(arrayQueues); free(arrayCounters);
	free(arrayNormals); free(arrayFunctions);
	free(arrayCombis);  json_value_free(root_value);
	if(arrayNodes)free(arrayNodes);
	//if(arrayPreceders)free(arrayPreceders);
	//if(arrayFollowers)free(arrayFollowers);
	return ret; 
}

/*
Valida El archivo de ingreso contra el schema y
 luego al archivo contra las reglas misma del modelo precursor
*/
int validateJsonInput( const char* filenameJson  ){

	int rta;
	if( (rta = validateSchema(filenameJson)) == VALIDATION_PASS )
		if( (rta = validateJson(filenameJson))== VALIDATION_PASS ){
			return VALIDATION_PASS;
		}
	
	printf("Validacion datos Json Fallido. Code=%d\n", rta);
	return rta;
}

/*
Valida El archivo de ingreso contra el schema
*/
int validateSchema(const char *filenameJson){
	const char* filenameSchema = "archivos/schema.json";
	FILE *jsonfile;
	FILE *schemafile;
	WJReader readjson;
	WJReader readschema;
	WJElement json;
	WJElement schema;
	//XplBool succ;
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
		printf("Aca falla!!");
		return INVALID_JSON_DEPTH;
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

/*
Handler de que hacer en caso de que falle la validación
*/
void schema_error(void *client, const char *format, ...)
{
	return;
}

/*
Handler de carga del validador
*/
WJElement schema_load(const char *name, void *client, const char *file, const int line)
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

		if( (schemafile = fopen(path, "r") )!= NULL) {
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


/*
Valida las reglas del modelo precursor
*/
int validateJson(const char *filenameJson){
	
	JSON_Value  *root_value = NULL;
    JSON_Object *object = NULL,*objectInArray = NULL;
    UNUSEDWARNING(objectInArray);
    JSON_Array  *array = NULL;
	UNUSEDWARNING(array);
	int i, j;
	UNUSEDWARNING(j);
	int sizeQueues, sizeCounters, sizeNormals, sizeFunctions, sizeCombis, sizeNodes;
	
	int *arrayQueues = NULL, *arrayCounters = NULL, *arrayNormals = NULL, *arrayFunctions = NULL, *arrayCombis = NULL,*arrayNodes = NULL;
	int sizePreceders = 0, sizeFollowers = 0;
	int *arrayPreceders = NULL, *arrayFollowers = NULL;
    /*VALIDATE JSON*/
    root_value = json_parse_file(filenameJson);
	
    if (!root_value) {
		return INVALID_JSON;
    }

    if (json_value_get_type(root_value) != JSONObject) {
    	json_value_free(root_value);
		return INVALID_JSON_OBJECT;
    }
    

    object = json_value_get_object(root_value);

	getArray(object, "transformation.queues"   , "idNode" ,&arrayQueues    , &sizeQueues);
	getArray(object, "transformation.counters" , "idNode" ,&arrayCounters  , &sizeCounters);
	getArray(object, "transformation.normals"  , "idNode" ,&arrayNormals   , &sizeNormals);
	getArray(object, "transformation.functions", "idNode" ,&arrayFunctions , &sizeFunctions);
	getArray(object, "transformation.combis"   , "idNode" ,&arrayCombis    , &sizeCombis);
	//for (i = 0; i < sizeNormals ; i++) printf("%d ",arrayNormals[i]); printf("\n");

	/*DIFERENTE NUMERO DE ID*/
	if (repeatArrays(arrayQueues, sizeQueues, arrayCounters, sizeCounters, arrayNormals, sizeNormals, arrayFunctions, sizeFunctions, arrayCombis, sizeCombis, &arrayNodes, &sizeNodes) == 0){
		return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL ID*/   
	}
	
	for (i = 0; i < sizeQueues ; i++)
	{
	    /*COLA: ANTESESOR: No pueden ser Colas.*/
		getArrayInArray(object, "transformation.queues", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayQueues,sizeQueues)> 0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_QUEUE); /*FAIL QUEUE*/
	    /*COLA: SUCESOR: Solo pueden ser combis.*/
		getArrayInArray(object, "transformation.queues", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) != sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_QUEUE); /*FAIL QUEUE*/
		/*COLA: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_QUEUE);/*FAIL QUEUE*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_QUEUE); /*FAIL QUEUE*/

		if(arrayPreceders)
			free(arrayPreceders);

		if(arrayFollowers)
			free(arrayFollowers);
			
		
		arrayPreceders = NULL;
		arrayFollowers = NULL;
		sizeFollowers = 0;
		sizePreceders = 0;
		
	}

	for (i = 0; i < sizeCombis ; i++)
	{
	    /*COMBI: ANTESESOR: Solo pueden ser nodos Cola.*/
		getArrayInArray(object, "transformation.combis", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayQueues,sizeQueues) !=  sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_COMBI); /*FAIL COMBI*/
    	/*COMBI: SUCESOR: No pueden ser Combis.*/
		getArrayInArray(object, "transformation.combis", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) >0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_COMBI); /*FAIL COMBI*/
		/*COMBI: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_COMBI); /*FAIL COMBI*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_COMBI); /*FAIL COMBI*/

		if(arrayPreceders)
			free(arrayPreceders);

		if(arrayFollowers)
			free(arrayFollowers);
			
		
		arrayPreceders = NULL;
		arrayFollowers = NULL;
		sizeFollowers = 0;
		sizePreceders = 0;
	}

	for (i = 0; i < sizeNormals ; i++)
	{
		/*NORMAL: ANTESESOR: No pueden ser colas*/
		getArrayInArray(object, "transformation.normals", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayQueues,sizeQueues)> 0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_NORMAL); /*FAIL NORMAL*/
    	/*NORMAL: SUCESOR: No pueden ser combis.*/
		getArrayInArray(object, "transformation.normals", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) >0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_NORMAL); /*FAIL NORMAL*/
		/*NORMAL: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_NORMAL); /*FAIL NORMAL*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_NORMAL); /*FAIL NORMAL*/

		if(arrayPreceders)
			free(arrayPreceders);

		if(arrayFollowers)
			free(arrayFollowers);
			
		
		arrayPreceders = NULL;
		arrayFollowers = NULL;
		sizeFollowers = 0;
		sizePreceders = 0;

	}

	for (i = 0; i < sizeFunctions ; i++)
	{
		/*FUNCION: ANTESESOR: No pueden ser colas*/
		getArrayInArray(object, "transformation.functions", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayQueues,sizeQueues)> 0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_FUNCTION); /*FAIL FUNCTION*/
    	/*FUNCION: SUCESOR: No pueden ser combis.*/
		getArrayInArray(object, "transformation.functions", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) >0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_FUNCTION); /*FAIL FUNCTION*/
		/*FUNCION: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_FUNCTION); /*FAIL FUNCION*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_FUNCTION); /*FAIL FUNCION*/

		if(arrayPreceders)
			free(arrayPreceders);

		if(arrayFollowers)
			free(arrayFollowers);
		
		arrayPreceders = NULL;
		arrayFollowers = NULL;
		sizeFollowers = 0;
		sizePreceders = 0;

	}

	for (i = 0; i < sizeCounters ; i++)
	{
    	/*CONTADOR: ANTESESOR: No pueden ser colas ni otro contador.*/
		getArrayInArray(object, "transformation.counters", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayQueues,sizeQueues)> 0 || countArrayInclude(arrayPreceders,sizePreceders,arrayCounters,sizeCounters)> 0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_COUNTER); /*FAIL CONTADOR*/
    	/*CONTADOR: SUCESOR: No pueden ser combis, ni contadores.*/
		getArrayInArray(object, "transformation.counters", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) >0 || countArrayInclude(arrayFollowers,sizeFollowers,arrayCounters,sizeCounters)> 0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_COUNTER); /*FAIL CONTADOR*/
		/*CONTADOR: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_COUNTER); /*FAIL CONTADOR*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, INVALID_COUNTER); /*FAIL CONTADOR*/

		if(arrayPreceders)
			free(arrayPreceders);

		if(arrayFollowers)
			free(arrayFollowers);
			
		
		arrayPreceders = NULL;
		arrayFollowers = NULL;
		sizeFollowers = 0;
		sizePreceders = 0;

	}

    return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_PASS);
}

/*
Arma un arreglo con los ids de un nodo X 
y anota el largo del arreglo en sizeArray
*/
void getArray( JSON_Object *objectJson, const char *arrayJson, const char *atributeJson, int** array, int* sizeArray ){
	int i;
	JSON_Array *arrayJsonFunction = json_object_dotget_array(objectJson, arrayJson);
    JSON_Object *objectInArray;
    *sizeArray = json_array_get_count(arrayJsonFunction);
    *array = (int*)malloc(sizeof(int) * (*sizeArray));

    for (i = 0; i < (*sizeArray); i++)
	{
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    (*array)[i] = json_object_dotget_number(objectInArray, atributeJson );
	}
}


/*
 * Ordena los elemntos de 'p' a 'r' 
 * en el arreglo A[...,p,..,r,...]
 */
void MergeSort(int p, int r, int** array){
	int q;
	 if(p < r){
		q = (p + r)/2;
		MergeSort(p, q,array);
		MergeSort(q + 1, r, array);
		Merge(p, q, r, array);
	}
}
/*
 * Mescla los elementos Desde A[p] y A[q]
 * hasta Desde A[q+1] y A[r]
 */
void Merge(int p, int q, int r, int** array){
	int* A = (*array);
	int i = p;
	int j = q+1;
	int k = 0;
	int* mem = (int*) malloc((p+r+1)*sizeof(int));

	while(i<=q && j<=r){
		if(A[i] <= A[j])
			mem[k++] =A[i++];
		else
			mem[k++] =A[j++];
	}
	while(i<=q) mem[k++] =A[i++];
	while(j<=r) mem[k++] =A[j++];

	for(i=0;i<=r-p;i++){
		A[p+i]=mem[i];//Lleno A
	}

	free(mem);
}

/*
Cuenta los nodos del modelo en  sizeArray
Y arma un arreglo con todos los ids en array
Y lo ordena para validarlo
*/
int repeatArrays(const int *const array1 ,const int sizeArray1, const int * const array2, const int sizeArray2, const int * const array3, const int sizeArray3, const int * const array4, const int sizeArray4, const int *const array5, const int sizeArray5, int** array, int* sizeArray){
	int i=0,j;
	*sizeArray = sizeArray1 + sizeArray2 + sizeArray3 + sizeArray4 + sizeArray5;
	*array = (int*)malloc(sizeof(int) * (*sizeArray));

	for (j = 0; j < sizeArray1; i++, j++)
		(*array)[i] = array1[j];
	for (j = 0; j < sizeArray2; i++, j++)
		(*array)[i] = array2[j];
	for (j = 0; j < sizeArray3; i++, j++)
		(*array)[i] = array3[j];
	for (j = 0; j < sizeArray4; i++, j++)
		(*array)[i] = array4[j];
	for (j = 0; j < sizeArray5; i++, j++)
		(*array)[i] = array5[j];

	MergeSort( 0, (*sizeArray)-1, array );
	
	
	
	if((*array)[0] != 1){// primer id distinto de 1
		printf(" (*array)[0] = %d \n", (*array)[0]);
		return 0;
	}
	for ( i = 0 ; i < ((*sizeArray)-1) ; i++){
		//detecto huecos y al mismo tiempo detecto repetidos
		if(  (*array)[i]+1 != (*array)[i+1] ){
			printf("i:%d :    (*array)[i]+1 = %d      !=    (*array)[i+1] = %d \n", i, (*array)[i]+1 , (*array)[i+1] );
			return 0;
		}
	}

	return (*sizeArray);
}

/*
Arma un arreglo con los ids precursores o sucesores del nodo i dentro del arreglo
y anota el largo del arreglo en sizeArray
*/
void getArrayInArray(JSON_Object * objectJson,const char *arrayJson,int pos,const char *arrayJsonIn, int** arreglo, int* countArreglo)
{
	JSON_Array *array = json_object_dotget_array(objectJson, arrayJson);
	JSON_Object *objectInArray = json_array_get_object(array, pos);
	JSON_Array *arrayJsonFunction = json_object_dotget_array(objectInArray, arrayJsonIn);
	int i;
    *countArreglo = json_array_get_count(arrayJsonFunction);
    if((*countArreglo))
		*arreglo = (int*)malloc(sizeof(int) * (*countArreglo));

    for (i = 0; i < (*countArreglo); i++)
	{
	    (*arreglo)[i] = json_array_get_number(arrayJsonFunction, i);
	}
}

/*
cuenta las coincidencias dentro de un arreglo contra otro
*/
int countArrayInclude(const int * const array, const int sizeArray, const int *const arrayFull, const int sizeArrayFull){
	int i,j, count=0;
    for (i = 0; i < sizeArray; i++)
	{
		for (j = 0; j < sizeArrayFull; j++)
		{
			if (array[i] == arrayFull[j])
			{
				count = count+1;
				break;
			}
		}
	}
	return count;
}


int getNodesAmount( const char *filenameJson ){
	int count = 0;
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);

	count += json_array_get_count( json_object_dotget_array(object, "transformation.queues") );
	count += json_array_get_count( json_object_dotget_array(object, "transformation.counters") );
 	count += json_array_get_count( json_object_dotget_array(object, "transformation.normals") );
	count += json_array_get_count( json_object_dotget_array(object, "transformation.functions") );
	count += json_array_get_count( json_object_dotget_array(object, "transformation.combis") );
	json_value_free(root_value);

	return count;
}

int* getCombiIds( const char *filenameJson ){
	int count = 0,i;
	int  *arrayCombis = NULL;
	int  sizeCombis;
	
	UNUSEDWARNING(arrayCombis);
	UNUSEDWARNING(sizeCombis);
	
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);
	JSON_Object *objectInArray;

	JSON_Array* arrayJsonFunction = json_object_dotget_array(object, "transformation.combis");

	count += json_array_get_count( arrayJsonFunction);
	int* ids = (int*)malloc(sizeof(int) * count+2);
	ids[0] = count+1; //combis + seed
	ids[1] = json_object_dotget_number (object, "seed");

	for (i = 0; i < count; i++){
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    ids[i+2] = json_object_dotget_number(objectInArray, "idNode" );
	}
	json_value_free(root_value);
	return ids;
}

int getWatchdog( const char *filenameJson ){
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);
	int ret = json_object_dotget_number (object, "length");
	json_value_free(root_value);
	return ret;
}

int* getTargets( const char *filenameJson ){
	int count = 0,i;
	int  *arrayCombis = NULL;
	int  sizeCombis;
	UNUSEDWARNING(arrayCombis);
	UNUSEDWARNING(sizeCombis);
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);
	JSON_Object *objectInArray;

	JSON_Array* arrayJsonFunction = json_object_dotget_array(object,"transformation.counters");

	count += json_array_get_count( arrayJsonFunction);
	int* targets = (int*)malloc(sizeof(int) * (count*2+1));
	targets[0] = count;
	
	for (i = 0; i < count; i++){
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    targets[i*2+1] = json_object_dotget_number(objectInArray, "idNode" );
	    targets[i*2+2] = json_object_dotget_number(objectInArray, "cycle" );
	}
	json_value_free(root_value);
	return targets;
}
