#include "jsonHelper.h"
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*  //Funcion principal
Valida El archivo de ingreso contra el schema y  luego al archivo contra las reglas misma del modelo precursor */
ValidationResults* validateJsonInput( const char* filenameJson ){
	int rta = validateSchema(filenameJson);
	if( rta == VALIDATION_PASS ) return validateJson(filenameJson);
	else{
		ValidationResults* validationResult = (ValidationResults*)malloc( sizeof(ValidationResults) );
		validationResult->isValid = rta;
		return validationResult;
	}
}

/**	Escribe el porque fracazo la validacion del json */
void writeErrorInFile(const char* label){
	char* botqueueOutputFile = getenv("BOTQUEUE_OUTPUT_FILE");
	if(botqueueOutputFile == NULL ){
		botqueueOutputFile = "/tmp/defaultOutputJson.json";
		printf("JsonHelper: can not find BOTQUEUE_OUTPUT_FILE, using default path: /tmp/defaultOutputJson.json \n");
	}
	int fileDescriptor = open (botqueueOutputFile, O_WRONLY|O_CREAT|O_TRUNC,00660);
	write(fileDescriptor,"{\n\"Error\" : \"",13);
	write(fileDescriptor, label, strlen(label) );
	write(fileDescriptor,"\"\n}",3);
	close(fileDescriptor);
}

/**	Escribe el porque fracazo la validacion del json */
void writeErrorInFileN(const char* label, const int N){
	char* botqueueOutputFile = getenv("BOTQUEUE_OUTPUT_FILE");
	if(botqueueOutputFile == NULL ){
		botqueueOutputFile = "/tmp/defaultOutputJson.json";
		printf("JsonHelper: can not find BOTQUEUE_OUTPUT_FILE, using default path: /tmp/defaultOutputJson.json \n");
	}
	int fileDescriptor = open (botqueueOutputFile,O_WRONLY|O_CREAT|O_TRUNC,00660);
	write(fileDescriptor,"{\n\"Error\" : \"",13);
	char* strNro = NULL;
	int len = snprintf(NULL, 0, label, N);
	strNro = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(strNro, (len + 1),label, N);
	write(fileDescriptor, strNro, len );
	write(fileDescriptor,"\"\n}",3);
	free(strNro);
	close(fileDescriptor);
}

/**	Escribe el porque fracazo la validacion del json */
void writeErrorInFileNS(const char* label, const int N, const char* nName){
	const char* withoutName = "Sin Nombre";
	char * rName;
	if (nName == NULL ){
		rName = (char *)withoutName;
	} else {
		rName = (char *)nName;
	}

	char* botqueueOutputFile = getenv("BOTQUEUE_OUTPUT_FILE");
	if(botqueueOutputFile == NULL ){
		botqueueOutputFile = "/tmp/defaultOutputJson.json";
		printf("JsonHelper: can not find BOTQUEUE_OUTPUT_FILE, using default path: /tmp/defaultOutputJson.json \n");
	}
	int fileDescriptor = open (botqueueOutputFile,O_WRONLY|O_CREAT|O_TRUNC,00660);
	write(fileDescriptor,"{\n\"Error\" : \"",13);
	char* strNro = NULL;

	int len = snprintf(NULL, 0, label, N, rName);
	strNro = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(strNro, (len + 1),label, N, rName);
	write(fileDescriptor, strNro, len );
	write(fileDescriptor,"\"\n}",3);
	free(strNro);
	close(fileDescriptor);
}

/**	Escribe el porque fracazo la validacion del json */
void writeErrorInFileNNNN(const char* label, const int N, const int M, const int O, const int P){
	char* botqueueOutputFile = getenv("BOTQUEUE_OUTPUT_FILE");
	if(botqueueOutputFile == NULL ){
		botqueueOutputFile = "/tmp/defaultOutputJson.json";
		printf("JsonHelper: can not find BOTQUEUE_OUTPUT_FILE, using default path: /tmp/defaultOutputJson.json \n");
	}
	int fileDescriptor = open (botqueueOutputFile,O_WRONLY|O_CREAT|O_TRUNC,00660);
	write(fileDescriptor,"{\n\"Error\" : \"",13);
	char* strNro = NULL;
	int len = snprintf(NULL, 0, label, N,M,O,P);
	strNro = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(strNro, (len + 1),label, N,M,O,P);
	write(fileDescriptor, strNro, len );
	write(fileDescriptor,"\"\n}",3);
	free(strNro);
	close(fileDescriptor);
}

/***********************************************REGION_SCHEMA*********************************************************/
/* Valida El archivo de ingreso contra el schema */
int validateSchema(const char *filenameJson){

	char* botqueueSchema = getenv("BOTQUEUE_SCHEMA");
	if(botqueueSchema == NULL ){
		botqueueSchema = "/tmp/schema.json";
		printf("JsonHelper: can not find BOTQUEUE_SCHEMA, using default path: /tmp/schema.json \n");
	}

	const char* filenameSchema = botqueueSchema;
	FILE *jsonfile; FILE *schemafile;
	WJReader readjson; 	WJReader readschema;
	WJElement json; WJElement schema; 	char *format=NULL;
	
	if(!(jsonfile = fopen(filenameJson, "r"))) {
		writeErrorInFile("No se encontro el archivo del modelo.");
		return ERROR_OPEN_JSON;
	}
	if(!(schemafile = fopen(filenameSchema, "r"))) {
		writeErrorInFile("No se encontro el archivo del esquema.");
		return ERROR_OPEN_SCHEMA;
	}

	if( !(readjson = WJROpenFILEDocument(jsonfile, NULL, 0)) ){
		writeErrorInFile("WJROpenFILEDocument No pudo leer el modelo.");
		return ERROR_READ_JSON;
	}
	if( !(json = WJEOpenDocument(readjson, NULL, NULL, NULL))) {
		writeErrorInFile("WJEOpenDocument No pudo leer el modelo.");
		return ERROR_READ_JSON;
	}
	if(!(readschema = WJROpenFILEDocument(schemafile, NULL, 0)) ){
		writeErrorInFile("WJROpenFILEDocument No pudo leer el esquema.");
		WJECloseDocument(json);
		return ERROR_READ_SCHEMA;
	}	
	if(!(schema = WJEOpenDocument(readschema, NULL, NULL, NULL))) {
		writeErrorInFile("WJEOpenDocument No pudo leer el esquema.");
		WJECloseDocument(json);
		return ERROR_READ_SCHEMA;
	}
	/*WJEDump(json);*/
	if (readjson->depth){
		writeErrorInFile("El json no es valido, Posiblemente falta una coma o una llave.");
		return INVALID_JSON_DEPTH;
	}
	/*WJEDump(schema);*/
	if (readschema->depth){
		writeErrorInFile("El json del esquema no es valido, Posiblemente falta una coma o una llave.");
		return INVALID_SCHEMA;
	}
	if(!WJESchemaValidate(schema, json, schema_error, schema_load, NULL, format)) {
		writeErrorInFile("El json es valido, pero no se respeta la estructura del esquema.");
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
/* Handler de que hacer en caso de que falle la validación */
void schema_error(void *client, const char *format, ...){
	return;
}
/* Handler de carga del validador */
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

/***********************************************REGION_SCHEMA*********************************************************/
/* Valida las reglas del modelo precursor */
ValidationResults*  validateJson(const char *filenameJson){

	ValidationResults* validationResult = (ValidationResults*)malloc( sizeof(ValidationResults) );
	
	JSON_Value  *root_value = NULL; JSON_Object *object = NULL;
    //cuantos nodes de cada uno hay
	int queueSize = 0, counterSize = 0, normalSize = 0, functionSize = 0, combiSize = 0, sizeAllNodes = 0;
	//cuales son los ids de cada uno
	int *queueArray = NULL, *counterArray = NULL, *normalArray = NULL, *functionArray = NULL, *combiArray = NULL,*allNodes = NULL;
	//allNodes asume el rol de todo tipo de nodo
	int **precederArrayFull = NULL; int **followerArrayFull = NULL;
	int precederSize = 0, followerSize = 0, probabilisticBranchSize = 0;
	
	double *probabilisticBranchArray = NULL;
	
    /*Se asume que el json ya esta validado por JWElement*/
    root_value = json_parse_file( filenameJson );
    object = json_value_get_object(root_value);
    
	queueSize = getIdNodeArray(object, "transformation.queues", &queueArray);
	counterSize = getIdNodeArray(object, "transformation.counters", &counterArray );
	normalSize = getIdNodeArray(object, "transformation.normals", &normalArray);
	functionSize = getIdNodeArray(object, "transformation.functions", &functionArray);
	combiSize = getIdNodeArray(object, "transformation.combis", &combiArray);

	sizeAllNodes = queueSize + counterSize + normalSize + functionSize + combiSize;

	/*Valida la que secuencia de id de los nodos sea 1,2,3,4...,N*/
	if (repeatArrays(queueArray, queueSize, counterArray, counterSize, normalArray, normalSize, functionArray, functionSize, combiArray, combiSize, &allNodes, &sizeAllNodes) == 0){
		writeErrorInFile("La secuencia de Ids es invalida: deben ser números consecutivos desde uno.");
		validationResult->isValid = VALIDATION_FAIL;
		goto freeIdNodeArray;
	}

	if( json_object_dotget_number (object, "length") <= 0 ){
		writeErrorInFile("El tiempo de simulacion debe ser mayor a 0");
		validationResult->isValid = VALIDATION_FAIL;
		goto freeIdNodeArray;	
	}

	if( TIME_ONE_STEP <= 0.0000001 ){
		writeErrorInFile("El paso de simulacion debe ser mayor a 0.0000001");
		validationResult->isValid = VALIDATION_FAIL;
		goto freeIdNodeArray;	
	}

	if( sizeAllNodes == 0 ) {
		writeErrorInFile("La cantidad de nodos en el mapa no puede ser cero.");
		validationResult->isValid = VALIDATION_FAIL;
		goto freeIdNodeArray;	
	}

	if( counterSize == 0 ) {
		writeErrorInFile("La cantidad de nodos counter en el mapa no puede ser cero.");
		validationResult->isValid = VALIDATION_FAIL;
		goto freeIdNodeArray;	
	}

	//Lista de punteros a proceders y followers segun id de nodo
	precederArrayFull = (int **) malloc(sizeAllNodes*sizeof(int*));
	followerArrayFull = (int **) malloc(sizeAllNodes*sizeof(int*));

	precederSize = getLinkTable(precederArrayFull, "transformation.queues", "preceders", object );
	precederSize += getLinkTable(precederArrayFull, "transformation.counters", "preceders", object );
	precederSize += getLinkTable(precederArrayFull, "transformation.normals", "preceders", object );
	precederSize += getLinkTable(precederArrayFull, "transformation.functions", "preceders", object );
	precederSize += getLinkTable(precederArrayFull, "transformation.combis", "preceders", object );
	followerSize = getLinkTable(followerArrayFull, "transformation.queues", "followers", object );
	followerSize += getLinkTable(followerArrayFull, "transformation.counters", "followers", object );
	followerSize += getLinkTable(followerArrayFull, "transformation.normals", "followers", object );
	followerSize += getLinkTable(followerArrayFull, "transformation.functions", "followers", object );
	followerSize += getLinkTable(followerArrayFull, "transformation.combis", "followers", object );


	if( precederSize != sizeAllNodes ) {
		writeErrorInFile("Los nodos en la lista de predecesores es diferente a la cantidad de nodos en el mapa");
		validationResult->isValid = VALIDATION_FAIL;
		goto freeLinksArray;	
	}

	if( followerSize != sizeAllNodes ) {
		writeErrorInFile("Los nodos en la lista de seguidores es diferente a la cantidad de nodos en el mapa");
		validationResult->isValid = VALIDATION_FAIL;
		goto freeLinksArray;	
	}

	//( indice-1 == idNode)
	if (validateDoubleReference(sizeAllNodes,precederArrayFull,followerArrayFull) == DOUBLE_REFERENCE_FAIL){
		//writeErrorInFile("No se cumple el principio de doble referencia, hay un nodo que apunta a otro nodo pero este esta recibiendo el link de otro nodo");
		validationResult->isValid = DOUBLE_REFERENCE_FAIL;
		goto freeLinksArray;
	}
	//( indice-1 == idNode) AVISO = El  writeErrorInFile esta dentro
	if (validateEachLinkIsUnique(sizeAllNodes,precederArrayFull) == VALIDATION_FAIL){
		validationResult->isValid = VALIDATION_FAIL;
		goto freeLinksArray;
	}
	//( indice-1 == idNode) AVISO = El  writeErrorInFile esta dentro
	if (validateAutoreference(sizeAllNodes,precederArrayFull) == AUTOREFERENCE_FAIL){
		validationResult->isValid = AUTOREFERENCE_FAIL;
		goto freeLinksArray;
	}
  	//AVISO = El  writeErrorInFile esta dentro
	if( VALIDATION_PASS != validateSeeds(object) ){
		validationResult->isValid = INVALID_MODEL_SEED;
		goto freeLinksArray;
	}
	int idNode;

	int *precederArray = NULL, *followerArray = NULL;
	for (int i = 0; i < queueSize ; i++){
		idNode = getIdNodeByPos( object, "transformation.queues", i);idNode--;
		precederArray = precederArrayFull[idNode]; 	followerArray = followerArrayFull[idNode];
	    /*COLA: ANTESESOR: No pueden ser Colas.*/
		if (disjointSet(precederArray,queueArray,queueSize) == FALSE){
			const char * nodeName = getNameByTypeAndId( object, "transformation.queues", idNode+1);
			writeErrorInFileNS("La cola id %d %s no puede preceder de otro nodo Cola.",idNode+1, nodeName);
			validationResult->isValid = INVALID_QUEUE;
			goto freeLinksArray;
		}

	    /*COLA: SUCESOR: Solo pueden ser combis.*/
		if ( (jointSet(followerArray,combiArray,combiSize) == FALSE) ||
		  (disjointSet(followerArray,queueArray,queueSize) == FALSE) ||
		  (disjointSet(followerArray,counterArray,counterSize) == FALSE) ||
		  (disjointSet(followerArray,normalArray,normalSize) == FALSE) ||
		  (disjointSet(followerArray,functionArray,functionSize) == FALSE) ){
			const char * nodeName = getNameByTypeAndId( object, "transformation.queues", idNode+1);
			writeErrorInFileNS("La cola id %d %s solo puede seguir a un nodo combi.",idNode+1, nodeName);
			validationResult->isValid = INVALID_QUEUE;
			goto freeLinksArray;
		}

		/*COLA: Existe referencia Preceders Followers*/
		if ( precederArrayFull[idNode][0] < 1 || followerArrayFull[idNode][0] < 1 ){
			const char * nodeName = getNameByTypeAndId( object, "transformation.queues", idNode+1);
			writeErrorInFileNS("La cola id %d %s debe tener al menos un antecesor y al menos un sucesor.",idNode+1, nodeName);
			validationResult->isValid = INVALID_QUEUE;
			goto freeLinksArray;
		}
	}

	for (int i = 0; i < combiSize ; i++){
		idNode = getIdNodeByPos( object, "transformation.combis", i);idNode--;
		precederArray = precederArrayFull[idNode]; 	followerArray = followerArrayFull[idNode];
	    /*COMBI: ANTESESOR: Solo pueden ser nodos Cola.*/
		if ( (jointSet(precederArray,queueArray,queueSize) == FALSE) ||
		  (disjointSet(precederArray,combiArray,combiSize) == FALSE) ||
		  (disjointSet(precederArray,counterArray,counterSize) == FALSE) ||
		  (disjointSet(precederArray,normalArray,normalSize) == FALSE) ||
		  (disjointSet(precederArray,functionArray,functionSize) == FALSE) ){
			const char * nodeName = getNameByTypeAndId( object, "transformation.combis", idNode+1);
			writeErrorInFileNS("La combi id %d %s solo puede preseder de nodos tipo cola.",idNode+1, nodeName);
			validationResult->isValid = INVALID_COMBI;
			goto freeLinksArray;
		}
    	/*COMBI: SUCESOR: No pueden ser Combis.*/
		if (disjointSet(followerArray,combiArray,combiSize) == FALSE){
			const char * nodeName = getNameByTypeAndId( object, "transformation.combis", idNode+1);
			writeErrorInFileNS("La combi id %d %s no puede seguir a un nodo tipo combi.",idNode+1, nodeName);
			validationResult->isValid = INVALID_COMBI;
			goto freeLinksArray;
		}
		/*COMBI: Existe referencia Preceders Followers*/
		if ( precederArrayFull[idNode][0]  < 1 || followerArrayFull[idNode][0]  < 1 ){
			const char * nodeName = getNameByTypeAndId( object, "transformation.combis", idNode+1);
			writeErrorInFileNS("La combi id %d %s debe tener al menos un antecesor y al menos un sucesor.",idNode+1, nodeName);
			validationResult->isValid = INVALID_COMBI;
			goto freeLinksArray;
		}
		getProbabilisticBranch(object, "transformation.combis", i,"probabilisticBranch",&probabilisticBranchArray , &probabilisticBranchSize);
		int error = validateProbabilisticBranch(probabilisticBranchArray, probabilisticBranchSize, followerArray[0]);
		if (probabilisticBranchArray) 
			free(probabilisticBranchArray);



		probabilisticBranchArray = NULL;
		probabilisticBranchSize = 0;
		if (error == PROBABILISTIC_BRANCH_FAIL){
			const char * nodeName = getNameByTypeAndId( object, "transformation.combis", idNode+1);
			writeErrorInFileNS("La sumatoria de la salida probabilistica del nodo combi id %d debe ser igual a 1",idNode+1, nodeName);
			validationResult->isValid = error;
			goto freeLinksArray;
		}

		if (error == PROBABILISTIC_BRANCH_COUNT_FAIL){
			const char * nodeName = getNameByTypeAndId( object, "transformation.combis", idNode+1);
			writeErrorInFileNS("La cantidad la salidas probabilisticas del nodo combi id %d debe ser igual al número de salidas.",idNode+1, nodeName);
			validationResult->isValid = error;
			goto freeLinksArray;
		}
	}

	for (int i = 0; i < normalSize ; i++){
		idNode = getIdNodeByPos( object,"transformation.normals", i);idNode--;
		precederArray = precederArrayFull[idNode]; followerArray = followerArrayFull[idNode];
		/*NORMAL: ANTESESOR: No pueden ser colas*/
		if (disjointSet(precederArray,queueArray,queueSize) == FALSE){
			const char * nodeName = getNameByTypeAndId( object, "transformation.normals", idNode+1);
			writeErrorInFileNS("La normal id %d %s no puede preseder de un nodo cola.",idNode+1, nodeName);
			validationResult->isValid = INVALID_NORMAL;
			goto freeLinksArray;
		}
    	/*NORMAL: SUCESOR: No pueden ser combis.*/
		if (disjointSet(followerArray,combiArray,combiSize) == FALSE){
			const char * nodeName = getNameByTypeAndId( object, "transformation.normals", idNode+1);
			writeErrorInFileNS("La normal id %d %s no puede seguir a una combi.",idNode+1, nodeName);
			validationResult->isValid = INVALID_NORMAL;
			goto freeLinksArray;
		}
		
		/*NORMAL: Existe referencia Preceders Followers*/
		if ( precederArrayFull[idNode][0]  < 1 || followerArrayFull[idNode][0]  < 1 ){
			const char * nodeName = getNameByTypeAndId( object, "transformation.normals", idNode+1);
			writeErrorInFileNS("La normal id %d %s debe tener al menos un antecesor y al menos un sucesor.",idNode+1, nodeName);
			validationResult->isValid = INVALID_NORMAL;
			goto freeLinksArray;
		}

		getProbabilisticBranch(object, "transformation.normals", i,"probabilisticBranch",&probabilisticBranchArray , &probabilisticBranchSize);
		int error = validateProbabilisticBranch(probabilisticBranchArray, probabilisticBranchSize, followerArray[0]);
		if (probabilisticBranchArray) 
			free(probabilisticBranchArray);
			
		probabilisticBranchArray = NULL;
		probabilisticBranchSize = 0;
		if (error == PROBABILISTIC_BRANCH_FAIL){
			const char * nodeName = getNameByTypeAndId( object, "transformation.normals", idNode+1);
			writeErrorInFileNS("La sumatoria de la salida probabilistica del nodo normal id %d debe ser igual a 1",idNode+1, nodeName);
			validationResult->isValid = error;
			goto freeLinksArray;
		}

		if (error == PROBABILISTIC_BRANCH_COUNT_FAIL){
			const char * nodeName = getNameByTypeAndId( object, "transformation.normals", idNode+1);
			writeErrorInFileNS("La cantidad la salidas probabilisticas del nodo normal id %d debe ser igual al número de salidas.",idNode+1, nodeName);
			validationResult->isValid = error;
			goto freeLinksArray;
		}
	}

	for (int i = 0; i < functionSize ; i++){
		idNode = getIdNodeByPos( object,"transformation.functions", i);idNode--;
		precederArray = precederArrayFull[idNode]; 	followerArray = followerArrayFull[idNode];
		/*FUNCION: ANTESESOR: No pueden ser colas*/
		if (disjointSet(precederArray,queueArray,queueSize) == FALSE){
			const char * nodeName = getNameByTypeAndId( object, "transformation.functions", idNode+1);
			writeErrorInFileNS("La función id %d %s no puede preceder de una cola.",idNode+1, nodeName);
			validationResult->isValid = INVALID_FUNCTION;
			goto freeLinksArray;
		}
    	/*FUNCION: SUCESOR: No pueden ser combis.*/
		if (disjointSet(followerArray,combiArray,combiSize) == FALSE){
			const char * nodeName = getNameByTypeAndId( object, "transformation.functions", idNode+1);
			writeErrorInFileNS("La función id %d %s no puede seguir a una combi.",idNode+1, nodeName);
			validationResult->isValid = INVALID_FUNCTION;
			goto freeLinksArray;
		}
		/*FUNCION: Existe referencia Preceders Followers*/
		if ( precederArrayFull[idNode][0]  < 1 || followerArrayFull[idNode][0]  < 1 ){
			const char * nodeName = getNameByTypeAndId( object, "transformation.functions", idNode+1);
			writeErrorInFileNS("La función id %d %s debe tener al menos un antecesor y al menos un sucesor.",idNode+1, nodeName);
			validationResult->isValid = INVALID_FUNCTION;
			goto freeLinksArray;
		}

		getProbabilisticBranch(object, "transformation.functions", i,"probabilisticBranch",&probabilisticBranchArray , &probabilisticBranchSize);
		int error = validateProbabilisticBranch(probabilisticBranchArray, probabilisticBranchSize, followerArray[0]);
		if (probabilisticBranchArray) 
			free(probabilisticBranchArray);
		
		printf("probabilisticBranchSize = %d\n", probabilisticBranchSize);

		probabilisticBranchArray = NULL;
		probabilisticBranchSize = 0;
		if (error == PROBABILISTIC_BRANCH_FAIL){
			const char * nodeName = getNameByTypeAndId( object, "transformation.functions", idNode+1);
			writeErrorInFileNS("La sumatoria de la salida probabilistica del nodo función id %d debe ser igual a 1",idNode+1, nodeName);
			validationResult->isValid = error;
			goto freeLinksArray;
		}

		if (error == PROBABILISTIC_BRANCH_COUNT_FAIL){
			const char * nodeName = getNameByTypeAndId( object, "transformation.functions", idNode+1);
			writeErrorInFileNS("La cantidad la salidas probabilisticas del nodo funcion id %d debe ser igual al número de salidas.",idNode+1, nodeName);
			validationResult->isValid = error;
			goto freeLinksArray;
		}
	}

	for (int i = 0; i < counterSize ; i++){
		idNode = getIdNodeByPos( object, "transformation.counters", i);idNode--;
		precederArray = precederArrayFull[idNode]; 	followerArray = followerArrayFull[idNode];
    	/*CONTADOR: ANTESESOR: No pueden ser colas ni otro contador.*/
		if ( disjointSet(precederArray,queueArray,queueSize) == FALSE || disjointSet(precederArray,counterArray,counterSize) == FALSE ){
			const char * nodeName = getNameByTypeAndId( object, "transformation.counters", idNode+1);
			writeErrorInFileNS("El contador idNode %d %s no puede preceder de una cola ni de un contador.",idNode+1, nodeName);
			validationResult->isValid = INVALID_COUNTER;
			goto freeLinksArray;
		}
    	/*CONTADOR: SUCESOR: No pueden ser combis, ni contadores.*/
    	if ( disjointSet(followerArray,combiArray,combiSize) == FALSE || disjointSet(followerArray,counterArray,counterSize) == FALSE){
    		const char * nodeName = getNameByTypeAndId( object, "transformation.counters", idNode+1);
			writeErrorInFileNS("El contador id %d %s no puede seguir a otro contador.",idNode+1, nodeName);
			validationResult->isValid = INVALID_COUNTER;
			goto freeLinksArray;
		}
		/*CONTADOR: Existe referencia Preceders Followers*/
		if ( precederArrayFull[idNode][0]  < 1 ){
			const char * nodeName = getNameByTypeAndId( object, "transformation.counters", idNode+1);
			writeErrorInFileNS("El contador id %d %s debe tener al menos un antecesor.",idNode+1, nodeName);
			validationResult->isValid = INVALID_COUNTER;
			goto freeLinksArray;
		}
	}

    validationResult->isValid = VALIDATION_PASS;
    validationResult->watchdog = json_object_dotget_number (object, "length");
    validationResult->seed = json_object_dotget_number (object, "seed");
    validationResult->seedAndCombisId = (int*)malloc(sizeof(int) * (combiSize+2) );
    validationResult->seedAndCombisId[0] = combiSize+1;
    validationResult->seedAndCombisId[1] = validationResult->seed;
    for(int i = 0; i < combiSize ; i++){
    	validationResult->seedAndCombisId[i+2] = combiArray[i];
    }
    validationResult->targetCounter = counterSize;
    validationResult->qCouNfComb = (int*)malloc(sizeof(int)*5);
    validationResult->qCouNfComb[0] = queueSize;
    validationResult->qCouNfComb[1] = counterSize;
    validationResult->qCouNfComb[2] = normalSize;
    validationResult->qCouNfComb[3] = functionSize;
    validationResult->qCouNfComb[4] = combiSize;
    validationResult->targets = getTargets( object);
    validationResult->nodesAmount = queueSize + counterSize + normalSize + functionSize + combiSize;

	freeLinksArray:
		for (int i = 0; i < sizeAllNodes; i++){
			//para cada nodo j en la listra de predecesores del nodo i
			if(precederArrayFull[i]) free(precederArrayFull[i]);
			if(followerArrayFull[i]) free(followerArrayFull[i]);
		}
		free(precederArrayFull);
		free(followerArrayFull);
	
    freeIdNodeArray: 
	    free(queueArray); free(counterArray);
		free(normalArray); free(functionArray);
		free(combiArray);  free(allNodes);
		json_value_free(root_value);
    return validationResult;
}
/***********************************************REGION_ID_SECUENCY*********************************************************/
/* Arma un arreglo con los ids (atributeJson == idNode) de un nodo X (arrayName) 
y anota el largo del arreglo en sizeArray */
int getIdNodeArray( JSON_Object *jsonObject, const char *arrayName, int** array ){
	JSON_Array *arrayJsonFunction = json_object_dotget_array(jsonObject, arrayName);
    JSON_Object *objectInArray;
    int sizeArray = json_array_get_count(arrayJsonFunction);
    *array = (int*)malloc(sizeof(int) * sizeArray);

    for (int i = 0; i < sizeArray; i++){
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    (*array)[i] = json_object_dotget_number(objectInArray, "idNode"  );
	}
	return sizeArray;
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
/* Ordena los elemntos de 'p' a 'r' 
 * en el arreglo A[...,p,..,r,...] */
void MergeSort(int firstElement, int lastElement, int** array){
	int middleElement;
	 if(firstElement < lastElement){
		middleElement = (firstElement + lastElement)/2;
		MergeSort(firstElement, middleElement,array);
		MergeSort(middleElement + 1, lastElement, array);
		Merge(firstElement, middleElement, lastElement, array);
	}
}
/* Mescla los elementos Desde A[p] y A[q]
 * hasta Desde A[q+1] y A[r] */
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
/***********************************************REGION_ID_SECUENCY*********************************************************/
/***********************************************REGION_DOUBLE_REFERENCE*********************************************************/
/* Obtiene la lista links (arrayJsonIn= followers o proceders)  de cada nodo (nodeName) y lo pone en una pocicion de una tabla segun el id "idNode" */
int getLinkTable(int** linkTable, const char *nodeName, const char *arrayJsonIn, JSON_Object *object ){
	JSON_Array  *arrayLocal = json_object_dotget_array(object, nodeName);
	int sizeLocalArray = json_array_get_count(arrayLocal);
	JSON_Object *objectInArray; JSON_Array *arrayJsonFunction;
	int linkCount, idNode;
	for(int pos = 0 ; pos < sizeLocalArray; pos++){
		objectInArray = json_array_get_object(arrayLocal, pos);
		idNode = (int)json_object_dotget_number(objectInArray, "idNode" );
		arrayJsonFunction = json_object_dotget_array(objectInArray, arrayJsonIn);

    	linkCount = json_array_get_count(arrayJsonFunction);
   		linkTable[idNode-1] = (int*) malloc((linkCount+1)*sizeof(int));

		linkTable[idNode-1][0] = linkCount;

		for (int j = 0; j < linkCount ; j++){
			linkTable[idNode-1][j+1] = json_array_get_number(arrayJsonFunction, j);
		}
	}
	return sizeLocalArray;
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
			if (isDoubleReference == FALSE){
				writeErrorInFileNNNN("Principio de doble referencia roto: Ambos nodos deben hacer referencia uno del otro.\nEl nodo id %d es precedido por el nodo id %d, pero el nodo id %d no sigue al nodo id %d", i+1, j+1, j+1,i+1 );
				return DOUBLE_REFERENCE_FAIL;
			}
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
			if (isDoubleReference == FALSE){
				writeErrorInFileNNNN("Principio de doble referencia roto: Ambos nodos deben hacer referencia uno del otro.\nEl nodo id %d sigue al nodo id %d, pero el nodo id %d no es precedido del nodo id %d", i+1, j+1, j+1,i+1 );
				return DOUBLE_REFERENCE_FAIL;
			}
		}
	}
	return VALIDATION_PASS;
}
/***********************************************REGION_DOUBLE_REFERENCE*********************************************************/
/***********************************************REGION_UNIQUE_REFERENCE*********************************************************/
/* cada link de uno a otro es unico(no hace falta validar el follower)*/
int validateEachLinkIsUnique(const int sizeAllNodes, int** precederArrayFull){
	int numberOfReferences;
	//para cada nodo i
	for (int i = 0; i < sizeAllNodes; i++){
		
		numberOfReferences = 0;
		//para cada nodo j en la listra de predecesores del nodo i
		for (int j = 1; j < precederArrayFull[i][0]+1 ; j++){
			//busco en si misma
			for (int k = 1; k < precederArrayFull[i][0]+1 ; k++){
				if (precederArrayFull[i][j] == precederArrayFull[i][k])
					numberOfReferences++;
			}
		}
		if(precederArrayFull[i][0] != numberOfReferences){
			writeErrorInFileN("El nodo con id %d apunta más de una vez a un nodo", i+1);
			return VALIDATION_FAIL;
		}
	}
	return VALIDATION_PASS;
}
/***********************************************REGION_UNIQUE_REFERENCE*********************************************************/
/***********************************************REGION_AUTO_REFERENCE*********************************************************/
/* Valida que un nodo no se apunte a si mismo en las referencias */

int validateAutoreference(const int sizeAllNodes, int** precederArrayFull){
	//para cada nodo i
	for (int i = 0; i < sizeAllNodes; i++){
		//para cada nodo j en la listra de predecesores del nodo i
		for (int j = 0; j < precederArrayFull[i][0] ; j++){
			//busco en si misma
			if(precederArrayFull[i][j+1] == i+1){
				writeErrorInFileN("El nodo con id %d se apunta a si mismo", i);
				return AUTOREFERENCE_FAIL;
			}
		}
	}
	return VALIDATION_PASS;
}	
/***********************************************REGION_AUTO_REFERENCE*********************************************************/
/* Valida que las semillas del modelo estan en el rango adecuado */
int validateSeeds(JSON_Object* object){
	int seed, combiSize, normalSize;
	JSON_Object * combi, * normal;
	JSON_Array* combiList,* normalList;
	
	seed = json_object_dotget_number (object, "seed");
	if(seed < -1){
		writeErrorInFile("El modelo no debe tener una semilla con valor menor a -1");
		return INVALID_MODEL_SEED;
	} 

	combiList = json_object_dotget_array(object, "transformation.combis");
	combiSize = json_array_get_count( combiList);
	
	for (int i = 0; i < combiSize; i++){
	    combi = json_array_get_object(combiList, i);
	    //si no es deterministica, testeo semilla
	    if (strcmp(json_object_dotget_string(combi,"delay.distribution"),"deterministic")!=0){
			seed = json_object_dotget_number(combi, "delay.seed" );
			if(seed < -1){
				writeErrorInFileN("El nodo combi %d-esimo, tiene tiene una semilla con valor menor a -1", i);
				return INVALID_MODEL_SEED;
			} 
		}
	}
	normalList = json_object_dotget_array(object, "transformation.normals");
	normalSize = json_array_get_count( normalList);
	for (int i = 0; i < normalSize; i++){
	    normal = json_array_get_object(normalList, i);
	    //si no es deterministica, testeo semilla
	    if (strcmp(json_object_dotget_string(normal,"delay.distribution"),"deterministic")!=0){
			seed = json_object_dotget_number(normal, "delay.seed" );
			if(seed < -1){
				writeErrorInFileN("El nodo normal %d-esimo, tiene tiene una semilla con valor menor a -1", i);
				return INVALID_MODEL_SEED;
			}
		}
	}
	return VALIDATION_PASS;
}
/*retorna el idNode*/
int getIdNodeByPos( JSON_Object *jsonObject, const char *arrayName, int pos ){
	JSON_Array *arrayJsonFunction = json_object_dotget_array(jsonObject, arrayName);
    JSON_Object *objectInArray  = json_array_get_object(arrayJsonFunction, pos);
	return json_object_dotget_number(objectInArray, "idNode"  );
}

/*El nodo X no tiene referencias */
int disjointSet(const int * const linkBag, const int *const singleTypeNodeBag, const int nodeBagSize){
	const int linkBagSize = linkBag[0];
    for (int i = 1; i <= linkBagSize; i++){
		for (int j = 0; j < nodeBagSize; j++){
			if (linkBag[i] == singleTypeNodeBag[j]){
				return FALSE;
			}
		}
	}
	return TRUE;
}

/*El nodo X solo tiene referencias justas*/
int jointSet(const int * const linkBag, const int *const singleTypeNodeBag, const int nodeBagSize){
	const int linkBagSize = linkBag[0];
	int found = FALSE;
    for (int i = 1; i <= linkBagSize; i++){
		for (int j = 0; j < nodeBagSize; j++){
			if (linkBag[i] == singleTypeNodeBag[j]){
				found = TRUE;
				break;
			}
		}
		if(found == FALSE)
			return FALSE;
	}
	return TRUE;
}
/***********************************************REGION_PROB_BRANCH*********************************************************/
/*Obtiene la lista de probabilidad de cada rama de un nodo */   
void getProbabilisticBranch(JSON_Object * objectJson,const char *arrayNodeName, const int pos,const char *arrayName, double** probabilisticBranchArray, int* probabilisticBranchSize){
	JSON_Array *array = json_object_dotget_array(objectJson, arrayNodeName);
	JSON_Object *objectInArray = json_array_get_object(array, pos);
	JSON_Array *arrayJsonFunction = json_object_dotget_array(objectInArray, arrayName);
    *probabilisticBranchSize = json_array_get_count(arrayJsonFunction);
    if((*probabilisticBranchSize))
		*probabilisticBranchArray = (double*)malloc(sizeof(double) * (*probabilisticBranchSize));
	printf("aaaaaaaaaaa\n");
    for (int i = 0; i < (*probabilisticBranchSize); i++){

	    (*probabilisticBranchArray)[i] = json_array_get_number(arrayJsonFunction, i);

	    printf("i = %d, valor= %lf\n", i,(*probabilisticBranchArray)[i]);
	}
}
/* Valida la probabilidad de cada rama y todas en conjunto*/
int validateProbabilisticBranch(const double* probabilisticBranch, const int probabilisticCounter, const int  followCounter){

	//si probabilisticCounter == 0  entonces esta disable la validacion
	if ( probabilisticCounter != 0  && probabilisticCounter != followCounter )
		return PROBABILISTIC_BRANCH_COUNT_FAIL;
	double cumulativeProbabilistic = 0.0;
	for (int j = 0 ; j < probabilisticCounter ; j++)
		cumulativeProbabilistic += probabilisticBranch[j];
	if ( probabilisticCounter != 0 ){
		if (cumulativeProbabilistic < PROB_MIN || cumulativeProbabilistic > PROB_MAX)
			return PROBABILISTIC_BRANCH_FAIL;
	}

	return VALIDATION_PASS;
}
/***********************************************REGION_PROB_BRANCH*********************************************************/
/* obtiene la cantidad de ciclos que deberia cumplir cada counter */
CycleValidator* getTargets( JSON_Object* object){
	int count;
	JSON_Object*  objectInArray;
	JSON_Array* arrayJsonFunction = json_object_dotget_array(object,"transformation.counters");
	count = json_array_get_count( arrayJsonFunction);
	CycleValidator* targets = (CycleValidator*)malloc(sizeof(CycleValidator) * count);
	for (int i = 0; i < count; i++){
	    objectInArray = json_array_get_object(arrayJsonFunction, i);
	    targets[i].idNode = json_object_dotget_number(objectInArray, "idNode" );
	    targets[i].cycle = json_object_dotget_number(objectInArray, "cycle" );
	}
	return targets;
}
/***********************************************REGION_NAMES*********************************************************/
const char * getNameByTypeAndId( JSON_Object* object, const char * nodeType, const int idNode){
	int currentNodeSize, currentId;
	JSON_Object * currentNode;
	JSON_Array* currentNodeList;
	currentNodeList = json_object_dotget_array(object, nodeType);
	currentNodeSize = json_array_get_count( currentNodeList);


	for (int i = 0; i < currentNodeSize; i++){
    	currentNode = json_array_get_object(currentNodeList, i);
		currentId = json_object_dotget_number(currentNode, "idNode" );
		if(currentId ==  idNode){
			return json_object_dotget_string(currentNode, "name" );
		} 
	}

	return NULL;
}
