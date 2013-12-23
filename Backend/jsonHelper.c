#include "jsonHelper.h"
#include <string.h>
/* 
limpia memoria y retorna un valor
*/
int freeAllAndReturn( int* queueArray, int* counterArray, int* normalArray, int* functionArray, int* combiArray, int* allNodes, JSON_Value* root_value, const int sizeAllNodes, int** precederArrayFull,int** followerArrayFull, const int ret){
	free(queueArray); free(counterArray);
	free(normalArray); free(functionArray);
	free(combiArray);  json_value_free(root_value);
	if(allNodes)free(allNodes);
	
	freeLinkTables(sizeAllNodes, precederArrayFull, followerArrayFull);
	return ret; 
}

/*
Valida El archivo de ingreso contra el schema y
 luego al archivo contra las reglas misma del modelo precursor
*/
int validateJsonInput( const char* filenameJson ){
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
	if (readjson->depth){
		printf("Fallo porque te olvidaste alguna coma\n");
		return INVALID_JSON_DEPTH;
	}

	/*WJEDump(schema);*/
	if (readschema->depth){
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
void schema_error(void *client, const char *format, ...){
	return;
}

/*
Handler de carga del validador
*/
WJElement schema_load(const char *name, void *client, const char *file, const int line){
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
    JSON_Object *object = NULL;
	int queueSize = 0, counterSize = 0, normalSize = 0,
	 functionSize = 0, combiSize = 0, sizeAllNodes = 0;
	int *queueArray = NULL, *counterArray = NULL, *normalArray = NULL,
	 *functionArray = NULL, *combiArray = NULL,*allNodes = NULL;
	int precederSize = 0, followerSize = 0, probabilisticBranchSize = 0;
	int *precederArray = NULL, *followerArray = NULL;
	double *probabilisticBranchArray = NULL;
	int **precederArrayFull = NULL;
	int **followerArrayFull = NULL;
	
    /*VALIDATE JSON*/
    root_value = json_parse_file( filenameJson );
	
    if (!root_value) {
		return INVALID_JSON;
    }

    if (json_value_get_type(root_value) != JSONObject) {
    	json_value_free(root_value);
		return INVALID_JSON_OBJECT;
    }
    
    object = json_value_get_object(root_value);
	
	getArray(object, "transformation.queues"   , "idNode" ,&queueArray    , &queueSize);
	getArray(object, "transformation.counters" , "idNode" ,&counterArray  , &counterSize);
	getArray(object, "transformation.normals"  , "idNode" ,&normalArray   , &normalSize);
	getArray(object, "transformation.functions", "idNode" ,&functionArray , &functionSize);
	getArray(object, "transformation.combis"   , "idNode" ,&combiArray    , &combiSize);

	/*DIFERENTE NUMERO DE ID*/
	if (repeatArrays(queueArray, queueSize, counterArray, counterSize, normalArray, normalSize, functionArray, functionSize, combiArray, combiSize, &allNodes, &sizeAllNodes) == 0){
		return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes , root_value,sizeAllNodes, precederArrayFull, followerArrayFull, VALIDATION_FAIL); /*FAIL ID*/   
	}

	//Lista de punteros a proceders y followers segun id de nodo menos 1
	precederArrayFull = (int **) malloc(sizeAllNodes*sizeof(int*));
	followerArrayFull = (int **) malloc(sizeAllNodes*sizeof(int*));

	for (int i = 0; i < queueSize ; i++){
	    /*COLA: ANTESESOR: No pueden ser Colas.*/
		getLink(object, "transformation.queues", i,"preceders",&precederArray , &precederSize);
		if (countArrayInclude(precederArray,precederSize,queueArray,queueSize)> 0)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_QUEUE); /*FAIL QUEUE*/
	    /*COLA: SUCESOR: Solo pueden ser combis.*/
		getLink(object, "transformation.queues", i,"followers",&followerArray , &followerSize);
		if (countArrayInclude(followerArray,followerSize,combiArray,combiSize) != followerSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_QUEUE); /*FAIL QUEUE*/
		/*COLA: Existe referencia Preceders Followers*/
		if (countArrayInclude(precederArray,precederSize,allNodes,sizeAllNodes)!= precederSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_QUEUE);/*FAIL QUEUE*/
		if (countArrayInclude(followerArray,followerSize,allNodes,sizeAllNodes)!= followerSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_QUEUE); /*FAIL QUEUE*/
		
		if ( validateEachLinkIsUnique(followerArray,followerSize) ==VALIDATION_FAIL || validateEachLinkIsUnique(precederArray,precederSize) ==VALIDATION_FAIL)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_QUEUE); /*FAIL QUEUE*/
			
		setLinksInTableOfLinks(object,precederArrayFull,"transformation.queues",i,precederArray , precederSize);
		setLinksInTableOfLinks(object,followerArrayFull,"transformation.queues",i,followerArray , followerSize);

		if(precederArray)
			free(precederArray);

		if(followerArray)
			free(followerArray);
			
		precederArray = NULL;
		followerArray = NULL;
		followerSize = 0;
		precederSize = 0;
	}

	for (int i = 0; i < combiSize ; i++){
	    /*COMBI: ANTESESOR: Solo pueden ser nodos Cola.*/
		getLink(object, "transformation.combis", i,"preceders",&precederArray , &precederSize);
		if (countArrayInclude(precederArray,precederSize,queueArray,queueSize) !=  precederSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_COMBI); /*FAIL COMBI*/
    	/*COMBI: SUCESOR: No pueden ser Combis.*/
		getLink(object, "transformation.combis", i,"followers",&followerArray , &followerSize);
		if (countArrayInclude(followerArray,followerSize,combiArray,combiSize) >0)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_COMBI); /*FAIL COMBI*/
		/*COMBI: Existe referencia Preceders Followers*/
		if (countArrayInclude(precederArray,precederSize,allNodes,sizeAllNodes)!= precederSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_COMBI); /*FAIL COMBI*/
		if (countArrayInclude(followerArray,followerSize,allNodes,sizeAllNodes)!= followerSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_COMBI); /*FAIL COMBI*/

		if ( validateEachLinkIsUnique(followerArray,followerSize) ==VALIDATION_FAIL || validateEachLinkIsUnique(precederArray,precederSize) ==VALIDATION_FAIL)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_COMBI); /*FAIL QUEUE*/

		getProbabilisticBranch(object, "transformation.combis", i,"probabilisticBranch",&probabilisticBranchArray , &probabilisticBranchSize);
		int error = validateProbabilisticBranch(probabilisticBranchArray, probabilisticBranchSize, followerSize);
		if (error != VALIDATION_PASS) return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, error); /*FAIL COMBI*/

		setLinksInTableOfLinks(object,precederArrayFull,"transformation.combis",i,precederArray , precederSize);
		setLinksInTableOfLinks(object,followerArrayFull,"transformation.combis",i,followerArray , followerSize);

		if(precederArray)
			free(precederArray);

		if(followerArray)
			free(followerArray);
			
		precederArray = NULL;
		followerArray = NULL;
		followerSize = 0;
		precederSize = 0;
		
		if (probabilisticBranchArray) 
			free(probabilisticBranchArray);
			
		probabilisticBranchArray = NULL;
		probabilisticBranchSize = 0;
	}

	for (int i = 0; i < normalSize ; i++){
		/*NORMAL: ANTESESOR: No pueden ser colas*/
		getLink(object, "transformation.normals", i,"preceders",&precederArray , &precederSize);
		if (countArrayInclude(precederArray,precederSize,queueArray,queueSize)> 0)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_NORMAL); /*FAIL NORMAL*/
    	/*NORMAL: SUCESOR: No pueden ser combis.*/
		getLink(object, "transformation.normals", i,"followers",&followerArray , &followerSize);
		if (countArrayInclude(followerArray,followerSize,combiArray,combiSize) >0)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_NORMAL); /*FAIL NORMAL*/
		/*NORMAL: Existe referencia Preceders Followers*/
		if (countArrayInclude(precederArray,precederSize,allNodes,sizeAllNodes)!= precederSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_NORMAL); /*FAIL NORMAL*/
		if (countArrayInclude(followerArray,followerSize,allNodes,sizeAllNodes)!= followerSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_NORMAL); /*FAIL NORMAL*/

		if ( validateEachLinkIsUnique(followerArray,followerSize) ==VALIDATION_FAIL || validateEachLinkIsUnique(precederArray,precederSize) ==VALIDATION_FAIL)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_NORMAL); /*FAIL QUEUE*/
			
		getProbabilisticBranch(object, "transformation.normals", i,"probabilisticBranch",&probabilisticBranchArray , &probabilisticBranchSize);
		int error = validateProbabilisticBranch(probabilisticBranchArray, probabilisticBranchSize, followerSize);
		if (error != VALIDATION_PASS) return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull, error); /*FAIL NORMAL*/

		if (validateAutoreference(object, "transformation.normals",i,precederArray , precederSize) == AUTOREFERENCE_FAIL) return AUTOREFERENCE_FAIL;
		if (validateAutoreference(object, "transformation.normals",i,followerArray , followerSize) == AUTOREFERENCE_FAIL) return AUTOREFERENCE_FAIL;
		setLinksInTableOfLinks(object,precederArrayFull,"transformation.normals",i,precederArray , precederSize);
		setLinksInTableOfLinks(object,followerArrayFull,"transformation.normals",i,followerArray , followerSize);

		if(precederArray)
			free(precederArray);

		if(followerArray)
			free(followerArray);
			
		
		precederArray = NULL;
		followerArray = NULL;
		followerSize = 0;
		precederSize = 0;
		
		if (probabilisticBranchArray) 
			free(probabilisticBranchArray);
			
		probabilisticBranchArray = NULL;
		probabilisticBranchSize = 0;
	}

	for (int i = 0; i < functionSize ; i++){
		/*FUNCION: ANTESESOR: No pueden ser colas*/
		getLink(object, "transformation.functions", i,"preceders",&precederArray , &precederSize);
		if (countArrayInclude(precederArray,precederSize,queueArray,queueSize)> 0)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_FUNCTION); /*FAIL FUNCTION*/
    	/*FUNCION: SUCESOR: No pueden ser combis.*/
		getLink(object, "transformation.functions", i,"followers",&followerArray , &followerSize);
		if (countArrayInclude(followerArray,followerSize,combiArray,combiSize) >0)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_FUNCTION); /*FAIL FUNCTION*/
		/*FUNCION: Existe referencia Preceders Followers*/
		if (countArrayInclude(precederArray,precederSize,allNodes,sizeAllNodes)!= precederSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_FUNCTION); /*FAIL FUNCION*/
		if (countArrayInclude(followerArray,followerSize,allNodes,sizeAllNodes)!= followerSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_FUNCTION); /*FAIL FUNCION*/

		if ( validateEachLinkIsUnique(followerArray,followerSize) ==VALIDATION_FAIL || validateEachLinkIsUnique(precederArray,precederSize) ==VALIDATION_FAIL)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_FUNCTION); /*FAIL QUEUE*/
			
		getProbabilisticBranch(object, "transformation.functions", i,"probabilisticBranch",&probabilisticBranchArray , &probabilisticBranchSize);
		int error = validateProbabilisticBranch(probabilisticBranchArray, probabilisticBranchSize, followerSize);
		if (error != VALIDATION_PASS) return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, error); /*FAIL FUNCTION*/

		if (validateAutoreference(object, "transformation.functions",i,precederArray , precederSize) == AUTOREFERENCE_FAIL) return AUTOREFERENCE_FAIL;
		if (validateAutoreference(object, "transformation.functions",i,followerArray , followerSize) == AUTOREFERENCE_FAIL) return AUTOREFERENCE_FAIL;
		setLinksInTableOfLinks(object,precederArrayFull,"transformation.functions",i,precederArray , precederSize);
		setLinksInTableOfLinks(object,followerArrayFull,"transformation.functions",i,followerArray , followerSize);

		if(precederArray)
			free(precederArray);

		if(followerArray)
			free(followerArray);
		
		precederArray = NULL;
		followerArray = NULL;
		followerSize = 0;
		precederSize = 0;
		
		if (probabilisticBranchArray) 
			free(probabilisticBranchArray);
			
		probabilisticBranchArray = NULL;
		probabilisticBranchSize = 0;
	}

	for (int i = 0; i < counterSize ; i++){
    	/*CONTADOR: ANTESESOR: No pueden ser colas ni otro contador.*/
		getLink(object, "transformation.counters", i,"preceders",&precederArray , &precederSize);
		if (countArrayInclude(precederArray,precederSize,queueArray,queueSize)> 0 || countArrayInclude(precederArray,precederSize,counterArray,counterSize)> 0)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_COUNTER); /*FAIL CONTADOR*/
    	/*CONTADOR: SUCESOR: No pueden ser combis, ni contadores.*/
		getLink(object, "transformation.counters", i,"followers",&followerArray , &followerSize);
		if (countArrayInclude(followerArray,followerSize,combiArray,combiSize) >0 || countArrayInclude(followerArray,followerSize,counterArray,counterSize)> 0)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_COUNTER); /*FAIL CONTADOR*/
		/*CONTADOR: Existe referencia Preceders Followers*/
		if (countArrayInclude(precederArray,precederSize,allNodes,sizeAllNodes)!= precederSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value,sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_COUNTER); /*FAIL CONTADOR*/
		if (countArrayInclude(followerArray,followerSize,allNodes,sizeAllNodes)!= followerSize)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_COUNTER); /*FAIL CONTADOR*/

		if ( validateEachLinkIsUnique(followerArray,followerSize) ==VALIDATION_FAIL || validateEachLinkIsUnique(precederArray,precederSize) ==VALIDATION_FAIL)
			return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,INVALID_COUNTER); /*FAIL QUEUE*/
			
		setLinksInTableOfLinks(object,precederArrayFull,"transformation.counters",i,precederArray , precederSize);
		setLinksInTableOfLinks(object,followerArrayFull,"transformation.counters",i,followerArray , followerSize);

		if(precederArray)
			free(precederArray);

		if(followerArray)
			free(followerArray);
			
		precederArray = NULL;
		followerArray = NULL;
		followerSize = 0;
		precederSize = 0;

	}

	//( indice-1 == idNode)
	if (validateDoubleReference(sizeAllNodes,precederArrayFull,followerArrayFull) == DOUBLE_REFERENCE_FAIL)
		return DOUBLE_REFERENCE_FAIL;
	
	if( VALIDATION_PASS != validateSeeds(object) )
		return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull, INVALID_MODEL_SEED);
		
    return freeAllAndReturn(queueArray, counterArray, normalArray, functionArray, combiArray, allNodes, root_value, sizeAllNodes, precederArrayFull, followerArrayFull,VALIDATION_PASS);
}

/*
 * VAlida la probabilidad de cada rama y todas en conjunto
 * */
int validateProbabilisticBranch(const double* probabilisticBranch, const int probabilisticCounter, const int  followCounter){
	if ( probabilisticCounter != 0  && probabilisticCounter != followCounter )
		return PROBABILISTIC_BRANCH_COUNT_FAIL;
	double cumulativeProbabilistic = 0.0;
	for (int j = 0 ; j < probabilisticCounter ; j++)
		cumulativeProbabilistic += probabilisticBranch[j];
	if ( probabilisticCounter > 0 )
		if (cumulativeProbabilistic < PROB_MIN || cumulativeProbabilistic > PROB_MAX)
			 return PROBABILISTIC_BRANCH_FAIL;
	return VALIDATION_PASS;
}

/*
 * Valida que un nodo no se apunte a si mismo en las referencias
 * */
int validateAutoreference(JSON_Object *object, const char *nodeName, const int pos, int* link, const int linkSize){
	int idNode;
	JSON_Array  *arrayLocal;
	JSON_Object *objectInArray;
	arrayLocal = json_object_dotget_array(object, nodeName);
	objectInArray = json_array_get_object(arrayLocal, pos);
	idNode = (int)json_object_dotget_number(objectInArray, "idNode" );

	for (int j=0 ; j < linkSize ; j++)
		if(link[j]==idNode)
			return AUTOREFERENCE_FAIL;
	return VALIDATION_PASS;
}		

/*
 * Obtiene la lista links de cada nodo y lo pone en una pocicion de una tabla segun el id
 * */
void setLinksInTableOfLinks(JSON_Object *object, int** linkTable, const char *nodeName, const int pos, int* linkArray, const int linkArraySize){
	int idNode,j;
	JSON_Array  *arrayLocal;
	JSON_Object *objectInArray;
	arrayLocal = json_object_dotget_array(object, nodeName);
	objectInArray = json_array_get_object(arrayLocal, pos);
	idNode = (int)json_object_dotget_number(objectInArray, "idNode" );
	linkTable[idNode-1] = (int*) malloc((linkArraySize+1)*sizeof(int));
	linkTable[idNode-1][0] = linkArraySize;
	for (j = 1; j <= linkTable[idNode-1][0] ; j++)
		linkTable[idNode-1][j] = linkArray[j-1];
}

int validateDoubleReference(const int sizeAllNodes, int** precederArrayFull,int** followerArrayFull){
	int isDoubleReference = TRUE;
	
	//para cada nodo i
	for (int i = 0; i < sizeAllNodes; i++){
		//para cada nodo j en la listra de predecesores del nodo i
		for (int j = 1; j <= precederArrayFull[i][0] ; j++){
			isDoubleReference = FALSE; // asumo que no esta y pruebo lo contrario
			
			//me busco en la lista sucesora en ese nodo k
			for ( int k = 1; k <= followerArrayFull[ precederArrayFull[i][j]-1 ][0] ; k++){
				if (followerArrayFull[ precederArrayFull[i][j]-1 ][k]-1 == i){
					isDoubleReference = TRUE;
					break;
				}
			}
			if (isDoubleReference == FALSE)
				return DOUBLE_REFERENCE_FAIL;
		}
		//para cada nodo j en la listra de sucesores del nodo i
		for (int j = 1; j <= followerArrayFull[i][0] ; j++){
			isDoubleReference = FALSE;// asumo que no esta y pruebo lo contrario
			
			//me busco en la lista de predecesores en ese nodo k
			for (int k = 1; k <= precederArrayFull[followerArrayFull[i][j]-1][0] ; k++){
				if (precederArrayFull[followerArrayFull[i][j]-1][k]-1 == i){
					isDoubleReference = TRUE;
					break;
				}
			}
			if (isDoubleReference == FALSE)
				return DOUBLE_REFERENCE_FAIL;
		}
	}
	return VALIDATION_PASS;
}

/*
 * Libero ambas tablas de links full
 * */
void freeLinkTables(const int sizeAllNodes, int** precederArrayFull,int** followerArrayFull){
	//para cada nodo i
	for (int i = 0; i < sizeAllNodes; i++){
		//para cada nodo j en la listra de predecesores del nodo i
		if(precederArrayFull[i]) free(precederArrayFull[i]);
		if(followerArrayFull[i]) free(followerArrayFull[i]);
	}
	if(precederArrayFull) free(precederArrayFull);
	precederArrayFull = NULL;
	if(followerArrayFull) free(followerArrayFull);
	followerArrayFull =NULL;
}

/*
Arma un arreglo con los ids de un nodo X 
y anota el largo del arreglo en sizeArray
*/
void getArray( JSON_Object *objectJson, const char *arrayName, const char *atributeJson, int** array, int* sizeArray ){
	JSON_Array *arrayJsonFunction = json_object_dotget_array(objectJson, arrayName);
    JSON_Object *objectInArray;
    *sizeArray = json_array_get_count(arrayJsonFunction);
    *array = (int*)malloc(sizeof(int) * (*sizeArray));

    for (int i = 0; i < (*sizeArray); i++){
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    (*array)[i] = json_object_dotget_number(objectInArray, atributeJson );
	}
}


/*
 * Ordena los elemntos de 'p' a 'r' 
 * en el arreglo A[...,p,..,r,...]
 */
void MergeSort(int firstElement, int lastElement, int** array){
	int middleElement;
	 if(firstElement < lastElement){
		middleElement = (firstElement + lastElement)/2;
		MergeSort(firstElement, middleElement,array);
		MergeSort(middleElement + 1, lastElement, array);
		Merge(firstElement, middleElement, lastElement, array);
	}
}
/*
 * Mescla los elementos Desde A[p] y A[q]
 * hasta Desde A[q+1] y A[r]
 */
void Merge(int firstElement, int middleElement, int lastElement, int** array){
	int* A = (*array);
	int i = firstElement;
	int j = middleElement+1;
	int k = 0;
	int* mem = (int*) malloc((firstElement + lastElement + 1)*sizeof(int));

	while(i <= middleElement && j <= lastElement){
		if(A[i] <= A[j])
			mem[k++] =A[i++];
		else
			mem[k++] =A[j++];
	}
	while(i <= middleElement) mem[k++] =A[i++];
	while(j <= lastElement) mem[k++] =A[j++];

	for(i=0;i <= lastElement - firstElement ;i++){
		A[firstElement + i]=mem[i];//Lleno A
	}

	free(mem);
}

/*
Cuenta los nodos del modelo en  sizeArray
Y arma un arreglo con todos los ids en array
Y lo ordena para validarlo
*/
int repeatArrays( const int *const queues ,const int queueSize,
				  const int *const counters, const int counterSize, 
				  const int * const normals, const int normalSize, 
				  const int * const functions, const int functionSize, 
				  const int *const combis, const int combiSize, 
				  int** allNodes, int* sizeAllNodes){
	int i = 0;
	*sizeAllNodes = queueSize + counterSize + normalSize + functionSize + combiSize;
	*allNodes = (int*)malloc(sizeof(int) * (*sizeAllNodes));

	for (int j = 0; j < queueSize; i++, j++)
		(*allNodes)[i] = queues[j];
	for (int j = 0; j < counterSize; i++, j++)
		(*allNodes)[i] = counters[j];
	for (int j = 0; j < normalSize; i++, j++)
		(*allNodes)[i] = normals[j];
	for (int j = 0; j < functionSize; i++, j++)
		(*allNodes)[i] = functions[j];
	for (int j = 0; j < combiSize; i++, j++)
		(*allNodes)[i] = combis[j];

	MergeSort( 0, (*sizeAllNodes)-1, allNodes );
	
	if((*allNodes)[0] != 1){// primer id distinto de 1
		return 0;
	}
	for ( i = 0 ; i < ((*sizeAllNodes)-1) ; i++){
		//detecto huecos y al mismo tiempo detecto repetidos
		if(  (*allNodes)[i]+1 != (*allNodes)[i+1] ){
			return 0;
		}
	}
	return (*sizeAllNodes);
}

/*
Arma un arreglo con los ids precursores o sucesores del nodo i dentro del arreglo
y anota el largo del arreglo en sizeArray
*/
void getLink(JSON_Object * objectJson,const char *arrayName, const int pos,const char *arrayJsonIn, int** nodeLinkList, int* linkCount)
{
	JSON_Array *array = json_object_dotget_array(objectJson, arrayName);
	JSON_Object *objectInArray = json_array_get_object(array, pos);
	JSON_Array *arrayJsonFunction = json_object_dotget_array(objectInArray, arrayJsonIn);
	int i;
    *linkCount = json_array_get_count(arrayJsonFunction);
    if((*linkCount))
		*nodeLinkList = (int*)malloc(sizeof(int) * (*linkCount));

    for (i = 0; i < (*linkCount); i++){
	    (*nodeLinkList)[i] = json_array_get_number(arrayJsonFunction, i);
	}
}

/*
 * Obtiene la lista de probabilidad de cada rama de un nodo
 * */   
void getProbabilisticBranch(JSON_Object * objectJson,const char *arrayNodeName, const int pos,const char *arrayName, double** probabilisticBranchArray, int* probabilisticBranchSize)
{
	JSON_Array *array = json_object_dotget_array(objectJson, arrayNodeName);
	JSON_Object *objectInArray = json_array_get_object(array, pos);
	JSON_Array *arrayJsonFunction = json_object_dotget_array(objectInArray, arrayName);
    *probabilisticBranchSize = json_array_get_count(arrayJsonFunction);
    if((*probabilisticBranchSize))
		*probabilisticBranchArray = (double*)malloc(sizeof(double) * (*probabilisticBranchSize));

    for (int i = 0; i < (*probabilisticBranchSize); i++){
	    (*probabilisticBranchArray)[i] = json_array_get_number(arrayJsonFunction, i);
	}
}

/*
cuenta las coincidencias dentro de un arreglo contra otro
*/
int countArrayInclude(const int * const linkBag, const int linkBagSize, const int *const singleTypeNodeBag, const int nodeBagSize){
	int count=0;
    for (int i = 0; i < linkBagSize; i++){
		for (int j = 0; j < nodeBagSize; j++){
			if (linkBag[i] == singleTypeNodeBag[j]){
				count++;
				break;
			}
		}
	}
	return count;
}


/*
cuenta las coincidencias dentro de un arreglo contra otro
*/
int validateEachLinkIsUnique(const int * const linkBag, const int linkBagSize){
	int count=0;
    for (int i = 0; i < linkBagSize; i++){
		for (int j = 0; j < linkBagSize; j++){
			if (linkBag[i] == linkBag[j]){
				count++;
			}
		}
	}
	if(linkBagSize == count)
		return VALIDATION_PASS;
	return VALIDATION_FAIL;
}


/*
 * Retorna la cantidad de nodos que hay en el modelo
 * */
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

/*
 * Retorna detalle de la cantidad de nodos que hay en el modelo
 * */
int* getNodesAmountDetail( const char *filenameJson ){
	int* count = (int*)malloc(sizeof(int)*5);
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);

	count[0] = json_array_get_count( json_object_dotget_array(object, "transformation.queues") );
	count[1] = json_array_get_count( json_object_dotget_array(object, "transformation.counters") );
 	count[2] = json_array_get_count( json_object_dotget_array(object, "transformation.normals") );
	count[3] = json_array_get_count( json_object_dotget_array(object, "transformation.functions") );
	count[4] = json_array_get_count( json_object_dotget_array(object, "transformation.combis") );
	json_value_free(root_value);

	return count;
}

/*
 * Obtiene los ids de todas las combis del modelo
 * */
int* getCombiIds( const char *filenameJson ){
	int count = 0;
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);
	JSON_Object *objectInArray;

	JSON_Array* arrayJsonFunction = json_object_dotget_array(object, "transformation.combis");

	count += json_array_get_count( arrayJsonFunction);
	int* ids = (int*)malloc(sizeof(int) * count+2);
	ids[0] = count+1; //combis + seed
	ids[1] = json_object_dotget_number (object, "seed");

	for (int i = 0; i < count; i++){
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    ids[i+2] = json_object_dotget_number(objectInArray, "idNode" );
	}
	json_value_free(root_value);
	return ids;
}

/*
 * Obtiene la cantidad de bucles maxima que no deberia superar el modelado
 * */
int getWatchdog( const char *filenameJson ){
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);
	int ret = json_object_dotget_number (object, "length");
	json_value_free(root_value);
	return ret;
}

/*
 * obtiene la semilla al principio del modelo
 * */
int getModelSeed( const char *filenameJson ){
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);
	int ret = json_object_dotget_number (object, "seed");
	json_value_free(root_value);
	return ret;
}

/*
 * Valida que las semillas del modelo estan en el rango adecuado
 * */
 
int validateSeeds(JSON_Object* object){
	int seed, combiSize, normalSize;
	JSON_Object * combi, * normal;
	JSON_Array* combiList,* normalList;
	
	seed = json_object_dotget_number (object, "seed");
	if(seed < -1) 
		return INVALID_MODEL_SEED;
	
	combiList = json_object_dotget_array(object, "transformation.combis");
	combiSize = json_array_get_count( combiList);
	
	for (int i = 0; i < combiSize; i++){
	    combi = json_array_get_object(combiList, i);
	    //si no es deterministica, testeo semilla
	    if (strcmp(json_object_dotget_string(combi,"delay.distribution"),"deterministic")!=0){
			seed = json_object_dotget_number(combi, "delay.seed" );
			if(seed < -1) 
				return INVALID_MODEL_SEED;
		}
	}
	normalList = json_object_dotget_array(object, "transformation.normals");
	normalSize = json_array_get_count( normalList);
	for (int i = 0; i < normalSize; i++){
	    normal = json_array_get_object(normalList, i);
	    //si no es deterministica, testeo semilla
	    if (strcmp(json_object_dotget_string(normal,"delay.distribution"),"deterministic")!=0){
			seed = json_object_dotget_number(normal, "delay.seed" );
			if(seed < -1) 
				return INVALID_MODEL_SEED;
		}
	}
	return VALIDATION_PASS;
}

/*
obtiene la cantidad de ciclos que deberia cumplir cada counter
*/
CycleValidator* getTargets( const char *filenameJson ){
	int count = 0;

	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);
	JSON_Object *objectInArray;

	JSON_Array* arrayJsonFunction = json_object_dotget_array(object,"transformation.counters");

	count += json_array_get_count( arrayJsonFunction);
	CycleValidator* targets = (CycleValidator*)malloc(sizeof(CycleValidator) * count);
	
	for (int i = 0; i < count; i++){
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    targets[i].idNode = json_object_dotget_number(objectInArray, "idNode" );
	    targets[i].cycle = json_object_dotget_number(objectInArray, "cycle" );
	}
	json_value_free(root_value);
	return targets;
}

int getCounterAcount( const char *filenameJson ){
	JSON_Value* root_value = json_parse_file(filenameJson);
	JSON_Object* object = json_value_get_object(root_value);
	JSON_Array* arrayJsonCounter = json_object_dotget_array(object,"transformation.counters");
	return json_array_get_count( arrayJsonCounter);
}
