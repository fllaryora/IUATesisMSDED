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
int validateJsonInput( const char* filenameJson , const char* filenameSchema ){

	int rta;
	if( (rta = validateSchema(filenameJson,filenameSchema)) == VALIDATION_PASS && (rta = validateJson(filenameJson))== VALIDATION_PASS )
		return VALIDATION_PASS;
	//si el primer termino de un and falla no se aplica el segundo, es decir nuna entra en 	validateJson, si falla el schema
	printf("Validacion datos Json Fallido. Code=%d\n", rta);
	return rta;
}

/*
Valida El archivo de ingreso contra el schema
*/
int validateSchema(const char *filenameJson, const char *filenameSchema)
{
	//const char* filenameSchema = "archivos/schema.json";
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
		return INVALID_JSON;
    }

    object = json_value_get_object(root_value);

	getArray(object, "transformation.queues"   , "idNode" ,&arrayQueues    , &sizeQueues);
	getArray(object, "transformation.counters" , "idNode" ,&arrayCounters  , &sizeCounters);
	getArray(object, "transformation.normals"  , "idNode" ,&arrayNormals   , &sizeNormals);
	getArray(object, "transformation.functions", "idNode" ,&arrayFunctions , &sizeFunctions);
	getArray(object, "transformation.combis"   , "idNode" ,&arrayCombis    , &sizeCombis);
	//for (i = 0; i < sizeNormals ; i++) printf("%d ",arrayNormals[i]); printf("\n");

	/*DIFERENTE NUMERO DE ID*/
	if (repeatArrays(arrayQueues, sizeQueues, arrayCounters, sizeCounters, arrayNormals, sizeNormals, arrayFunctions, sizeFunctions, arrayCombis, sizeCombis, &arrayNodes, &sizeNodes) > 0)
		return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL ID*/   

	for (i = 0; i < sizeQueues ; i++)
	{
	    /*COLA: ANTESESOR: No pueden ser Colas.*/
		getArrayInArray(object, "transformation.queues", i,"preceders",&arrayPreceders , &sizePreceders);
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayQueues,sizeQueues)> 0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL QUEUE*/
	    /*COLA: SUCESOR: Solo pueden ser combis.*/
		getArrayInArray(object, "transformation.queues", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) != sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL QUEUE*/
		/*COLA: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL);/*FAIL QUEUE*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL QUEUE*/

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
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL);; /*FAIL COMBI*/
    	/*COMBI: SUCESOR: No pueden ser Combis.*/
		getArrayInArray(object, "transformation.combis", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) >0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL COMBI*/
		/*COMBI: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL COMBI*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL COMBI*/

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
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL NORMAL*/
    	/*NORMAL: SUCESOR: No pueden ser combis.*/
		getArrayInArray(object, "transformation.normals", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) >0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL NORMAL*/
		/*NORMAL: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL NORMAL*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL NORMAL*/

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
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL FUNCTION*/
    	/*FUNCION: SUCESOR: No pueden ser combis.*/
		getArrayInArray(object, "transformation.functions", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) >0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL FUNCTION*/
		/*FUNCION: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL FUNCION*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL FUNCION*/

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
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL CONTADOR*/
    	/*CONTADOR: SUCESOR: No pueden ser combis, ni contadores.*/
		getArrayInArray(object, "transformation.counters", i,"followers",&arrayFollowers , &sizeFollowers);
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayCombis,sizeCombis) >0 || countArrayInclude(arrayFollowers,sizeFollowers,arrayCounters,sizeCounters)> 0)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL CONTADOR*/
		/*CONTADOR: Existe referencia Preceders Followers*/
		if (countArrayInclude(arrayPreceders,sizePreceders,arrayNodes,sizeNodes)!= sizePreceders)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL CONTADOR*/
		if (countArrayInclude(arrayFollowers,sizeFollowers,arrayNodes,sizeNodes)!= sizeFollowers)
			return freeAllAndReturn(arrayQueues, arrayCounters, arrayNormals, arrayFunctions, arrayCombis, arrayNodes, arrayPreceders, arrayFollowers , root_value, VALIDATION_FAIL); /*FAIL CONTADOR*/

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
Cuenta los nodos del modelo en  sizeArray
Y arma un arreglo con todos los ids en array
Y lo ordena para validarlo
*/
int repeatArrays(const int *const array1 ,const int sizeArray1, const int * const array2, const int sizeArray2, const int * const array3, const int sizeArray3, const int * const array4, const int sizeArray4, const int *const array5, const int sizeArray5, int** array, int* sizeArray){
	int i=0,j,count=0;
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

	for (i = 0; i < *sizeArray ; i++)
		for (j = i+1; j < *sizeArray ; j++)
	    	if ((*array)[i] == (*array)[j])
				count=count+1;

	return count;
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


void sendStructToNodes( const char *filenameJson )
{
	Queue 	 *queues;	int queuesCount;
	Counter  *counters;	int counterCount;
	Function *functions;int functionCount;
	Normal	 *normals;	int normalCount;
	Combi 	 *combis;	int combiCount;

	// LECTURA DE ESTRUCTURAS
	getQueues(filenameJson,&queues, &queuesCount);
	getCounters(filenameJson,&counters, &counterCount);
	getFunctions(filenameJson,&functions, &functionCount);
	getNormals(filenameJson,&normals, &normalCount);
	getCombis(filenameJson,&combis, &combiCount);

	// ENVIO DE STRUCTURAS
	sendStruct(&queues, &queuesCount,&counters, &counterCount,&functions, &functionCount,&normals, &normalCount, &combis, &combiCount);

	return;
}

void sendStruct(Queue **queues, int *queuesCount,Counter **counters, int *counterCount,Function **functions, int *functionCount,Normal **normals, int *normalCount,Combi **combis, int *combiCount)
{
	int i,j=0;

	// ENVIO DE 'QUEUES' (2 ENVIOS ADICIONALES PARA 'PRECEDERS' Y 'FOLLOWERS')
	for (i=0 ; i < *queuesCount ; i++) //QUEUE
	{
		MPI_Send(&((*queues)[i]), sizeof(Queue),  MPI_BYTE, i+MASTER_RAFFLER_PRINTER, QUEUE, MPI_COMM_WORLD);
		if ((*queues)[i].countPreceders>0)
			MPI_Send((*queues)[i].preceders, (*queues)[i].countPreceders ,  MPI_INT, i+MASTER_RAFFLER_PRINTER, QUEUE, MPI_COMM_WORLD);
		if ((*queues)[i].countFollowers>0)
			MPI_Send((*queues)[i].followers, (*queues)[i].countFollowers ,  MPI_INT, i+MASTER_RAFFLER_PRINTER, QUEUE, MPI_COMM_WORLD);
	}
	j+=i;
	// ENVIO DE NORMAL
	for (i=0 ; i < *normalCount ; i++)
	{
		MPI_Send(&((*normals)[i]), sizeof(Normal),  MPI_BYTE, i+j+MASTER_RAFFLER_PRINTER, NORMAL, MPI_COMM_WORLD);
		if ((*normals)[i].countPreceders>0)
			MPI_Send((*normals)[i].preceders, (*normals)[i].countPreceders ,  MPI_INT, i+j+MASTER_RAFFLER_PRINTER, NORMAL, MPI_COMM_WORLD);
		if ((*normals)[i].countFollowers>0)
			MPI_Send((*normals)[i].followers, (*normals)[i].countFollowers ,  MPI_INT, i+j+MASTER_RAFFLER_PRINTER, NORMAL, MPI_COMM_WORLD);
		if ((*normals)[i].countProbabilisticBranch>0)
			MPI_Send((*normals)[i].probabilisticBranch, (*normals)[i].countProbabilisticBranch ,  MPI_DOUBLE, i+j+MASTER_RAFFLER_PRINTER, NORMAL, MPI_COMM_WORLD);
	}
	j+=i;

	// ENVIO DE COUNTER
	for (i=0 ; i < *counterCount ; i++)
	{
		MPI_Send(&((*counters)[i]), sizeof(Counter),  MPI_BYTE, i+j+MASTER_RAFFLER_PRINTER, COUNTER, MPI_COMM_WORLD);
		if ((*counters)[i].countPreceders>0)
			MPI_Send((*counters)[i].preceders, (*counters)[i].countPreceders ,  MPI_INT, i+j+MASTER_RAFFLER_PRINTER, COUNTER, MPI_COMM_WORLD);
		if ((*counters)[i].countFollowers>0)
			MPI_Send((*counters)[i].followers, (*counters)[i].countFollowers ,  MPI_INT, i+j+MASTER_RAFFLER_PRINTER, COUNTER, MPI_COMM_WORLD);
	}
	j+=i;

	// ENVIO DE FUNCTION
	for (i=0 ; i < *functionCount ; i++)
	{
		MPI_Send(&((*functions)[i]), sizeof(Function),  MPI_BYTE, i+j+MASTER_RAFFLER_PRINTER, FUNCTION, MPI_COMM_WORLD);
		if ((*functions)[i].countPreceders>0)
			MPI_Send((*functions)[i].preceders, (*functions)[i].countPreceders ,  MPI_INT, i+j+MASTER_RAFFLER_PRINTER, FUNCTION, MPI_COMM_WORLD);
		if ((*functions)[i].countFollowers>0)
			MPI_Send((*functions)[i].followers, (*functions)[i].countFollowers ,  MPI_INT, i+j+MASTER_RAFFLER_PRINTER, FUNCTION, MPI_COMM_WORLD);
		if ((*functions)[i].countProbabilisticBranch>0)
			MPI_Send((*functions)[i].probabilisticBranch, (*functions)[i].countProbabilisticBranch ,  MPI_DOUBLE, i+j+MASTER_RAFFLER_PRINTER, FUNCTION, MPI_COMM_WORLD);
	}
	j+=i;

	// ENVIO DE 'COMBIS' (3 ENVIOS ADICIONALES PARA 'PRECEDERS', 'FOLLOWERS' y 'PROBABILISTIC_BRANCH')
	for (i=0 ; i < *combiCount ; i++)
	{
		MPI_Send(&((*combis)[i]), sizeof(Combi),  MPI_BYTE, i+j+MASTER_RAFFLER_PRINTER, COMBI, MPI_COMM_WORLD);
		if ((*combis)[i].countPreceders>0)
			MPI_Send((*combis)[i].preceders, (*combis)[i].countPreceders ,  MPI_INT, i+j+MASTER_RAFFLER_PRINTER, COMBI, MPI_COMM_WORLD);
		if ((*combis)[i].countFollowers>0)
			MPI_Send((*combis)[i].followers, (*combis)[i].countFollowers ,  MPI_INT, i+j+MASTER_RAFFLER_PRINTER, COMBI, MPI_COMM_WORLD);
		if ((*combis)[i].countProbabilisticBranch>0)
			MPI_Send((*combis)[i].probabilisticBranch, (*combis)[i].countProbabilisticBranch ,  MPI_DOUBLE, i+j+MASTER_RAFFLER_PRINTER, COMBI, MPI_COMM_WORLD);
	}
	j+=i;
}

void getQueues(const char *filenameJson , Queue **queues, int *queuesCount)
{
    JSON_Value  *root_value;
    JSON_Object *object,*objectInArray;
    JSON_Array  *array,*arrayInternal;
	int i, j;

    root_value = json_parse_file(filenameJson);
    object = json_value_get_object(root_value);
	array = json_object_dotget_array(object, "transformation.queues");

	*queuesCount = json_array_get_count(array);
	*queues = (Queue *) malloc((*queuesCount)*sizeof(Queue));

    for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    (*queues)[i].idNode = (int)json_object_dotget_number(objectInArray, "idNode" );
	   	(*queues)[i].resource = (int)json_object_dotget_number(objectInArray, "resource" );
		(*queues)[i].fixedCost = json_object_dotget_number(objectInArray, "fixedCost" );
		(*queues)[i].variableCost = json_object_dotget_number(objectInArray, "variableCost" );

		arrayInternal = json_object_dotget_array(objectInArray, "preceders");
		(*queues)[i].countPreceders = json_array_get_count(arrayInternal);
		(*queues)[i].preceders = (int *) malloc((*queues)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*queues)[i].countPreceders; j++)
			(*queues)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*queues)[i].countFollowers = json_array_get_count(arrayInternal);
		(*queues)[i].followers = (int *) malloc((*queues)[i].countFollowers*sizeof(int));
		for (j = 0; j < (*queues)[i].countFollowers; j++)
			(*queues)[i].followers[j]=json_array_get_number(arrayInternal,j);
	}
}

void getCounters(const char *filenameJson , Counter **counters, int *counterCount)
{
	JSON_Value  *root_value;
    JSON_Object *object,*objectInArray;
    JSON_Array  *array,*arrayInternal;
	int i, j;

    root_value = json_parse_file(filenameJson);
    object = json_value_get_object(root_value);
	array = json_object_dotget_array(object, "transformation.counters");

	*counterCount = json_array_get_count(array);
	*counters = (Counter *) malloc((*counterCount)*sizeof(Counter));

    for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    (*counters)[i].idNode = (int)json_object_dotget_number(objectInArray, "idNode" );
	   	(*counters)[i].quantity = (int)json_object_dotget_number(objectInArray, "quantity" );
		(*counters)[i].cycle = json_object_dotget_number(objectInArray, "cycle" );
		arrayInternal = json_object_dotget_array(objectInArray, "preceders");
		(*counters)[i].countPreceders = json_array_get_count(arrayInternal);
		(*counters)[i].preceders = (int *) malloc((*counters)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*counters)[i].countPreceders; j++)
			(*counters)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*counters)[i].countFollowers = json_array_get_count(arrayInternal);
		(*counters)[i].followers = (int *) malloc((*counters)[i].countFollowers*sizeof(int));
		for (j = 0; j < (*counters)[i].countFollowers; j++)
			(*counters)[i].followers[j]=json_array_get_number(arrayInternal,j);
	}
}

void getFunctions(const char *filenameJson , Function **functions, int *functionCount)
{
		JSON_Value  *root_value;
    JSON_Object *object,*objectInArray;
    JSON_Array  *array,*arrayInternal;
	int i, j;

    root_value = json_parse_file(filenameJson);
    object = json_value_get_object(root_value);
	array = json_object_dotget_array(object, "transformation.functions");

	*functionCount = json_array_get_count(array);
	*functions = (Function *) malloc((*functionCount)*sizeof(Function));

    for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    (*functions)[i].idNode = (int)json_object_dotget_number(objectInArray, "idNode" );
	   	(*functions)[i].input = (int)json_object_dotget_number(objectInArray, "input" );
	   	(*functions)[i].output = (int)json_object_dotget_number(objectInArray, "output" );

		arrayInternal = json_object_dotget_array(objectInArray, "preceders");
		(*functions)[i].countPreceders = json_array_get_count(arrayInternal);
		(*functions)[i].preceders = (int *) malloc((*functions)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*functions)[i].countPreceders; j++)
			(*functions)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*functions)[i].countFollowers = json_array_get_count(arrayInternal);
		(*functions)[i].followers = (int *) malloc((*functions)[i].countFollowers*sizeof(int));
		for (j = 0; j < (*functions)[i].countFollowers; j++)
			(*functions)[i].followers[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "probabilisticBranch");
		(*functions)[i].countProbabilisticBranch = json_array_get_count(arrayInternal);
		(*functions)[i].probabilisticBranch = (double *) malloc((*functions)[i].countProbabilisticBranch*sizeof(double));
		for (j = 0; j < (*functions)[i].countProbabilisticBranch; j++)
			(*functions)[i].probabilisticBranch[j]=json_array_get_number(arrayInternal,j);
	}
}

void getNormals(const char *filenameJson , Normal **normals, int *normalCount)
{
	JSON_Value  *root_value;
    JSON_Object *object,*objectInArray, *objectDelay;
    JSON_Array  *array,*arrayInternal;
	int i, j;

    root_value = json_parse_file(filenameJson);
    object = json_value_get_object(root_value);
	array = json_object_dotget_array(object, "transformation.normals");

	*normalCount = json_array_get_count(array);
	*normals = (Normal *) malloc((*normalCount)*sizeof(Normal));

	for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    (*normals)[i].idNode = (int)json_object_dotget_number(objectInArray, "idNode" );

		arrayInternal = json_object_dotget_array(objectInArray, "preceders");
		(*normals)[i].countPreceders = json_array_get_count(arrayInternal);
		(*normals)[i].preceders = (int *) malloc((*normals)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*normals)[i].countPreceders; j++)
			(*normals)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*normals)[i].countFollowers = json_array_get_count(arrayInternal);
		(*normals)[i].followers = (int *) malloc((*normals)[i].countFollowers*sizeof(int));
		for (j = 0; j < (*normals)[i].countFollowers; j++)
			(*normals)[i].followers[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "probabilisticBranch");
		(*normals)[i].countProbabilisticBranch = json_array_get_count(arrayInternal);
		(*normals)[i].probabilisticBranch = (double *) malloc((*normals)[i].countProbabilisticBranch*sizeof(double));
		for (j = 0; j < (*normals)[i].countProbabilisticBranch; j++)
			(*normals)[i].probabilisticBranch[j]=json_array_get_number(arrayInternal,j);

		objectDelay = json_object_dotget_object(objectInArray, "delay" ); 

		if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"uniform")==0)
		{
			(*normals)[i].delay.distribution = DIST_UNIFORM;
			(*normals)[i].delay.least = json_object_dotget_number(objectDelay, "least" );
			(*normals)[i].delay.highest = json_object_dotget_number(objectDelay, "highest" );
			(*normals)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"deterministic")==0)
		{
			(*normals)[i].delay.distribution = DIST_DETERMINISTIC;
			(*normals)[i].delay.constant = json_object_dotget_number(objectDelay, "constant" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"normal")==0)
		{
			(*normals)[i].delay.distribution = DIST_NORMAL;
			(*normals)[i].delay.mean = json_object_dotget_number(objectDelay, "mean" );
			(*normals)[i].delay.variance = json_object_dotget_number(objectDelay, "variance" );
			(*normals)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"exponential")==0)
		{
			(*normals)[i].delay.distribution = DIST_EXPONENTIAL;
			(*normals)[i].delay.lambda = json_object_dotget_number(objectDelay, "lambda" );
			(*normals)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"triangular")==0)
		{
			(*normals)[i].delay.distribution = DIST_TRIANGULAR;
			(*normals)[i].delay.least = json_object_dotget_number(objectDelay, "least" );
			(*normals)[i].delay.highest = json_object_dotget_number(objectDelay, "highest" );
			(*normals)[i].delay.mode = json_object_dotget_number(objectDelay, "mode" );
			(*normals)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"beta")==0)
		{
			(*normals)[i].delay.distribution = DIST_BETA;
			(*normals)[i].delay.minimun = json_object_dotget_number(objectDelay, "minimun" );
			(*normals)[i].delay.maximun = json_object_dotget_number(objectDelay, "maximun" );
			(*normals)[i].delay.shapeAlpha = json_object_dotget_number(objectDelay, "shapeAlpha" );
			(*normals)[i].delay.shapeBeta = json_object_dotget_number(objectDelay, "shapeBeta" );
			(*normals)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"log-normal")==0)
		{
			(*normals)[i].delay.distribution = DIST_LOG_NORMAL;
			(*normals)[i].delay.escale = json_object_dotget_number(objectDelay, "escale" );
			(*normals)[i].delay.shape = json_object_dotget_number(objectDelay, "shape" );
			(*normals)[i].delay.minimun = json_object_dotget_number(objectDelay, "minimun" );
			(*normals)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}

		/*switch( json_object_dotget_object(objectInArray, "delay" ) ){
			case "normal":
				break;
			case "uniform":
				break;
		}*/
	}
}

void getCombis(const char *filenameJson , Combi **combis, int *combiCount)
{
	JSON_Value  *root_value;
    JSON_Object *object,*objectInArray, *objectDelay;
    JSON_Array  *array,*arrayInternal;
	int i, j;


    root_value = json_parse_file(filenameJson);
    object = json_value_get_object(root_value);
	array = json_object_dotget_array(object, "transformation.combis");

	*combiCount = json_array_get_count(array);
	*combis = (Combi *) malloc((*combiCount)*sizeof(Combi));

	for (i = 0; i < json_array_get_count(array); i++)
	{
	    objectInArray = json_array_get_object(array, i);
	    (*combis)[i].idNode = (int)json_object_dotget_number(objectInArray, "idNode" );

		arrayInternal = json_object_dotget_array(objectInArray, "preceders");
		(*combis)[i].countPreceders = json_array_get_count(arrayInternal);
		(*combis)[i].preceders = (int *) malloc((*combis)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*combis)[i].countPreceders; j++)
			(*combis)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*combis)[i].countFollowers = json_array_get_count(arrayInternal);
		(*combis)[i].followers = (int *) malloc((*combis)[i].countFollowers*sizeof(int));
		for (j = 0; j < (*combis)[i].countFollowers; j++)
			(*combis)[i].followers[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "probabilisticBranch");
		(*combis)[i].countProbabilisticBranch = json_array_get_count(arrayInternal);
		(*combis)[i].probabilisticBranch = (double *) malloc((*combis)[i].countProbabilisticBranch*sizeof(double));
		for (j = 0; j < (*combis)[i].countProbabilisticBranch; j++)
			(*combis)[i].probabilisticBranch[j]=json_array_get_number(arrayInternal,j);

		(*combis)[i].delay.distribution = DIST_UNIFORM; //uniform

		objectDelay = json_object_dotget_object(objectInArray, "delay" ); 

		if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"uniform")==0)
		{
			(*combis)[i].delay.distribution = DIST_UNIFORM;
			(*combis)[i].delay.least = json_object_dotget_number(objectDelay, "least" );
			(*combis)[i].delay.highest = json_object_dotget_number(objectDelay, "highest" );
			(*combis)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"deterministic")==0)
		{
			(*combis)[i].delay.distribution = DIST_DETERMINISTIC;
			(*combis)[i].delay.constant = json_object_dotget_number(objectDelay, "constant" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"normal")==0)
		{
			(*combis)[i].delay.distribution = DIST_NORMAL;
			(*combis)[i].delay.mean = json_object_dotget_number(objectDelay, "mean" );
			(*combis)[i].delay.variance = json_object_dotget_number(objectDelay, "variance" );
			(*combis)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"exponential")==0)
		{
			(*combis)[i].delay.distribution = DIST_EXPONENTIAL;
			(*combis)[i].delay.lambda = json_object_dotget_number(objectDelay, "lambda" );
			(*combis)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"triangular")==0)
		{
			(*combis)[i].delay.distribution = DIST_TRIANGULAR;
			(*combis)[i].delay.least = json_object_dotget_number(objectDelay, "least" );
			(*combis)[i].delay.highest = json_object_dotget_number(objectDelay, "highest" );
			(*combis)[i].delay.mode = json_object_dotget_number(objectDelay, "mode" );
			(*combis)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"beta")==0)
		{
			(*combis)[i].delay.distribution = DIST_BETA;
			(*combis)[i].delay.minimun = json_object_dotget_number(objectDelay, "minimun" );
			(*combis)[i].delay.maximun = json_object_dotget_number(objectDelay, "maximun" );
			(*combis)[i].delay.shapeAlpha = json_object_dotget_number(objectDelay, "shapeAlpha" );
			(*combis)[i].delay.shapeBeta = json_object_dotget_number(objectDelay, "shapeBeta" );
			(*combis)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}
		else if (strcmp(json_object_dotget_string(objectDelay,"distribution"),"log-normal")==0)
		{
			(*combis)[i].delay.distribution = DIST_LOG_NORMAL;
			(*combis)[i].delay.escale = json_object_dotget_number(objectDelay, "escale" );
			(*combis)[i].delay.shape = json_object_dotget_number(objectDelay, "shape" );
			(*combis)[i].delay.minimun = json_object_dotget_number(objectDelay, "minimun" );
			(*combis)[i].delay.seed = json_object_dotget_number(objectDelay, "seed" );
		}

		/*switch( json_object_dotget_object(objectInArray, "delay" ) ){
			case "normal":
				break;
			case "uniform":
				break;
		}*/
	}
}

void printQueue(Queue queue)
{
	int i;

	printf("%d: idNode: %d\n", queue.idNode, queue.idNode);
	printf("%d: resource: %d\n", queue.idNode, queue.resource);
	printf("%d: fixedCost: %.4f\n", queue.idNode, queue.fixedCost);
	printf("%d: variableCost: %.4f\n", queue.idNode, queue.variableCost);
	printf("%d: countPreceders: %d\n", queue.idNode, queue.countPreceders);
	printf("%d: countFollowers: %d\n", queue.idNode, queue.countFollowers);

	for (i=0 ; i<queue.countPreceders ; i++)
		printf("%d: preceders[%d]: %d\n", queue.idNode,i,queue.preceders[i]);

	for (i=0 ; i<queue.countFollowers ; i++)
		printf("%d: followers[%d]: %d\n", queue.idNode,i,queue.followers[i]);
}

void printCounter(Counter counter){
	int i;

	printf("%d: idNode: %d\n", counter.idNode, counter.idNode);
	printf("%d: quantity: %d\n", counter.idNode, counter.quantity);
	printf("%d: cycle: %.2f\n", counter.idNode, counter.cycle);
	printf("%d: countPreceders: %d\n", counter.idNode, counter.countPreceders);
	printf("%d: countFollowers: %d\n", counter.idNode, counter.countFollowers);

	for (i=0 ; i<counter.countPreceders ; i++)
		printf("%d: preceders[%d]: %d\n", counter.idNode,i,counter.preceders[i]);

	for (i=0 ; i<counter.countFollowers ; i++)
		printf("%d: followers[%d]: %d\n", counter.idNode,i,counter.followers[i]);
}

void printFunction(Function function)
{
	int i;

	printf("%d: idNode: %d\n", function.idNode, function.idNode);
	printf("%d: input: %d\n", function.idNode, function.input);
	printf("%d: output: %d\n", function.idNode, function.output);
	printf("%d: countPreceders: %d\n", function.idNode, function.countPreceders);
	printf("%d: countFollowers: %d\n", function.idNode, function.countFollowers);
	printf("%d: countProbabilisticBranch: %d\n", function.idNode, function.countProbabilisticBranch);

	for (i=0 ; i<function.countPreceders ; i++)
		printf("%d: preceders[%d]: %d\n", function.idNode,i,function.preceders[i]);

	for (i=0 ; i<function.countFollowers ; i++)
		printf("%d: followers[%d]: %d\n", function.idNode,i,function.followers[i]);

	for (i=0 ; i<function.countProbabilisticBranch ; i++)
		printf("%d: probabilisticBranch[%d]: %.2f\n", function.idNode, i,function.probabilisticBranch[i]);
}

void printNormal(Normal normal)
{
	int i;

	printf("%d: idNode: %d\n", normal.idNode,normal.idNode);
	printf("%d: countPreceders: %d\n", normal.idNode, normal.countPreceders);
	printf("%d: countFollowers: %d\n", normal.idNode, normal.countFollowers);
	printf("%d: countProbabilisticBranch: %d\n", normal.idNode, normal.countProbabilisticBranch);

	for (i=0 ; i<normal.countPreceders ; i++)
		printf("%d: preceders[%d]: %d\n", normal.idNode, i,normal.preceders[i]);

	for (i=0 ; i<normal.countFollowers ; i++)
		printf("%d: followers[%d]: %d\n", normal.idNode, i,normal.followers[i]);

	for (i=0 ; i<normal.countProbabilisticBranch ; i++)
		printf("%d: probabilisticBranch[%d]: %.2f\n", normal.idNode, i,normal.probabilisticBranch[i]);

	if (normal.delay.distribution == DIST_UNIFORM)
	{
		printf("%d: delay.distribution: 'uniform'\n",normal.idNode);
		printf("%d: delay.least: %.4f\n",normal.idNode,normal.delay.least);
		printf("%d: delay.highest: %.4f\n",normal.idNode,normal.delay.highest);
		printf("%d: delay.seed: %d\n",normal.idNode,normal.delay.seed);
	}
	else if (normal.delay.distribution == DIST_DETERMINISTIC)
	{
		printf("%d: delay.distribution: 'deterministic'\n",normal.idNode);
		printf("%d: delay.constant: %.4f\n",normal.idNode,normal.delay.constant);
	}
	else if (normal.delay.distribution == DIST_NORMAL)
	{
		printf("%d: delay.distribution: 'normal'\n",normal.idNode);
		printf("%d: delay.mean: %.4f\n",normal.idNode,normal.delay.mean);
		printf("%d: delay.variance: %.4f\n",normal.idNode,normal.delay.variance);
		printf("%d: delay.seed: %d\n",normal.idNode,normal.delay.seed);
	}
	else if (normal.delay.distribution == DIST_EXPONENTIAL)
	{
		printf("%d: delay.distribution: 'exponential'\n",normal.idNode);
		printf("%d: delay.lambda: %.4f\n",normal.idNode,normal.delay.lambda);
		printf("%d: delay.seed: %d\n",normal.idNode,normal.delay.seed);
	}
	else if (normal.delay.distribution == DIST_TRIANGULAR)
	{
		printf("%d: delay.distribution: 'triangular'\n",normal.idNode);
		printf("%d: delay.least: %.4f\n",normal.idNode,normal.delay.least);
		printf("%d: delay.highest: %.4f\n",normal.idNode,normal.delay.highest);
		printf("%d: delay.mode: %.4f\n",normal.idNode,normal.delay.mode);
		printf("%d: delay.seed: %d\n",normal.idNode,normal.delay.seed);
	}
	else if (normal.delay.distribution == DIST_BETA)
	{
		printf("%d: delay.distribution: 'beta'\n",normal.idNode);
		printf("%d: delay.minimun: %.4f\n",normal.idNode,normal.delay.minimun);
		printf("%d: delay.maximun: %.4f\n",normal.idNode,normal.delay.maximun);
		printf("%d: delay.shapeAlpha: %.4f\n",normal.idNode,normal.delay.shapeAlpha);
		printf("%d: delay.shapeBeta: %.4f\n",normal.idNode,normal.delay.shapeBeta);
		printf("%d: delay.seed: %d\n",normal.idNode,normal.delay.seed);
	}
	else if (normal.delay.distribution == DIST_LOG_NORMAL)
	{
		printf("%d: delay.distribution: 'log-normal'\n",normal.idNode);
		printf("%d: delay.escale: %.4f\n",normal.idNode,normal.delay.escale);
		printf("%d: delay.shape: %.4f\n",normal.idNode,normal.delay.shape);
		printf("%d: delay.minimun: %.4f\n",normal.idNode,normal.delay.minimun);
		printf("%d: delay.seed: %d\n",normal.idNode,normal.delay.seed);
	}
}

void printCombi(Combi combi)
{
	int i;

	printf("%d: idNode: %d\n", combi.idNode,combi.idNode);
	printf("%d: countPreceders: %d\n", combi.idNode, combi.countPreceders);
	printf("%d: countFollowers: %d\n", combi.idNode, combi.countFollowers);
	printf("%d: countProbabilisticBranch: %d\n", combi.idNode, combi.countProbabilisticBranch);

	for (i=0 ; i<combi.countPreceders ; i++)
		printf("%d: preceders[%d]: %d\n", combi.idNode, i,combi.preceders[i]);

	for (i=0 ; i<combi.countFollowers ; i++)
		printf("%d: followers[%d]: %d\n", combi.idNode, i,combi.followers[i]);

	for (i=0 ; i<combi.countProbabilisticBranch ; i++)
		printf("%d: probabilisticBranch[%d]: %.2f\n", combi.idNode, i,combi.probabilisticBranch[i]);

	if (combi.delay.distribution == DIST_UNIFORM)
	{
		printf("%d: delay.distribution: 'uniform'\n",combi.idNode);
		printf("%d: delay.least: %.4f\n",combi.idNode,combi.delay.least);
		printf("%d: delay.highest: %.4f\n",combi.idNode,combi.delay.highest);
		printf("%d: delay.seed: %d\n",combi.idNode,combi.delay.seed);
	}
	else if (combi.delay.distribution == DIST_DETERMINISTIC)
	{
		printf("%d: delay.distribution: 'deterministic'\n",combi.idNode);
		printf("%d: delay.constant: %.4f\n",combi.idNode,combi.delay.constant);
	}
	else if (combi.delay.distribution == DIST_NORMAL)
	{
		printf("%d: delay.distribution: 'normal'\n",combi.idNode);
		printf("%d: delay.mean: %.4f\n",combi.idNode,combi.delay.mean);
		printf("%d: delay.variance: %.4f\n",combi.idNode,combi.delay.variance);
		printf("%d: delay.seed: %d\n",combi.idNode,combi.delay.seed);
	}
	else if (combi.delay.distribution == DIST_EXPONENTIAL)
	{
		printf("%d: delay.distribution: 'exponential'\n",combi.idNode);
		printf("%d: delay.lambda: %.4f\n",combi.idNode,combi.delay.lambda);
		printf("%d: delay.seed: %d\n",combi.idNode,combi.delay.seed);
	}
	else if (combi.delay.distribution == DIST_TRIANGULAR)
	{
		printf("%d: delay.distribution: 'triangular'\n",combi.idNode);
		printf("%d: delay.least: %.4f\n",combi.idNode,combi.delay.least);
		printf("%d: delay.highest: %.4f\n",combi.idNode,combi.delay.highest);
		printf("%d: delay.mode: %.4f\n",combi.idNode,combi.delay.mode);
		printf("%d: delay.seed: %d\n",combi.idNode,combi.delay.seed);
	}
	else if (combi.delay.distribution == DIST_BETA)
	{
		printf("%d: delay.distribution: 'beta'\n",combi.idNode);
		printf("%d: delay.minimun: %.4f\n",combi.idNode,combi.delay.minimun);
		printf("%d: delay.maximun: %.4f\n",combi.idNode,combi.delay.maximun);
		printf("%d: delay.shapeAlpha: %.4f\n",combi.idNode,combi.delay.shapeAlpha);
		printf("%d: delay.shapeBeta: %.4f\n",combi.idNode,combi.delay.shapeBeta);
		printf("%d: delay.seed: %d\n",combi.idNode,combi.delay.seed);
	}
	else if (combi.delay.distribution == DIST_LOG_NORMAL)
	{
		printf("%d: delay.distribution: 'log-normal'\n",combi.idNode);
		printf("%d: delay.escale: %.4f\n",combi.idNode,combi.delay.escale);
		printf("%d: delay.shape: %.4f\n",combi.idNode,combi.delay.shape);
		printf("%d: delay.minimun: %.4f\n",combi.idNode,combi.delay.minimun);
		printf("%d: delay.seed: %d\n",combi.idNode,combi.delay.seed);
	}
}
