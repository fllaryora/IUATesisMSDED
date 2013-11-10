#include "ourMPI.h"
#include "main.h"
#include "genericNode.h"
#include "queue.h"
#include "combi.h"
#include "jsonHelper.h"
#include <stdio.h>
#include <stdlib.h>

void sendStructToNodes( const char *filenameJson ,const MPI_Comm commNodes)
{
	Queue  *queues = NULL;	int queuesCount = 0;
	Counter  *counters = NULL;	int counterCount = 0;
	Function *functions = NULL;int functionCount = 0;
	Normal	 *normals = NULL;	int normalCount = 0;
	Combi 	 *combis = NULL;	int combiCount = 0;

	// LECTURA DE ESTRUCTURAS
	getQueues(filenameJson,&queues, &queuesCount);
	getCounters(filenameJson,&counters, &counterCount);
	getFunctions(filenameJson,&functions, &functionCount);
	getNormals(filenameJson,&normals, &normalCount);
	getCombis(filenameJson,&combis, &combiCount);

	// ENVIO DE STRUCTURAS
	sendStruct(commNodes,queues, queuesCount,counters, counterCount,functions, functionCount,normals, normalCount, combis, combiCount);
	//Libero memoria
	if(queues){
		for (int i = 0; i < queuesCount; i++){
			if(queues[i].countPreceders > 0) free( queues[i].preceders ) ;
			if(queues[i].countFollowers > 0) free( queues[i].followers );
		}
		free(queues);
	}
	
	if(counters){
		for (int i = 0; i < counterCount; i++){
			if(counters[i].countPreceders > 0) free( counters[i].preceders );
			if(counters[i].countFollowers > 0) free( counters[i].followers );
		}
		free(counters);
	}
	
	if(functions){

		for (int i = 0; i < functionCount; i++){
			if(functions[i].countPreceders > 0) free( functions[i].preceders );
			if(functions[i].countFollowers > 0) free( functions[i].followers );
			if(functions[i].countProbabilisticBranch > 0) free(functions[i].probabilisticBranch );
		}
		free(functions);
	}
	
	if(normals){
		for (int i = 0; i < normalCount; i++){
			if(normals[i].countPreceders > 0) free( normals[i].preceders);
			if(normals[i].countFollowers > 0) free(normals[i].followers);
			if(normals[i].countProbabilisticBranch > 0) free(normals[i].probabilisticBranch);
		}
		free(normals);
	}
	
	if(combis){
		for (int i = 0; i < combiCount; i++){
			if(combis[i].countPreceders > 0) free(combis[i].preceders);
			if(combis[i].countFollowers > 0) free(combis[i].followers);
			if(combis[i].countProbabilisticBranch > 0) free(combis[i].probabilisticBranch);
		}
		free(combis);
	}
	
	return;
}

void genericNode(const int myIdNodo,const int  idNodoInterno,const MPI_Comm commNodes, const int mpiProcesses){
	int msg;
	MPI_Status status;
	Queue queue;
	Counter counter;
	Function function;
	Normal normal;
	Combi combi;

	// RECIBO ESTRUCTURA PARTICULAR, DEJO DE SER NODO_GENERICO
	MPI_Probe( 0, MPI_ANY_TAG, commNodes, &status);
	if (status.MPI_TAG == QUEUE)
	{
		receiveQueue(commNodes, &queue);
		//printQueue(queue);
		queueNode(  commNodes, &queue, mpiProcesses);
		
		if(queue.countPreceders > 0) free( queue.preceders );
		if(queue.countFollowers > 0) free( queue.followers );
	}
		else if (status.MPI_TAG == COUNTER)
	{
		receiveCounter(commNodes, &counter);
		//printCounter(counter);
		//TODO provisorio
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
		if(counter.countPreceders > 0) free( counter.preceders );
		if(counter.countFollowers > 0) free( counter.followers );
		
	}
	else if (status.MPI_TAG == NORMAL)
	{
		receiveNormal(commNodes, &normal);
		//printNormal(normal);
		//TODO provisorio
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
		if(normal.countPreceders > 0) free( normal.preceders);
		if(normal.countFollowers > 0) free(normal.followers);
		if(normal.countProbabilisticBranch > 0) free(normal.probabilisticBranch);
	}
	else if (status.MPI_TAG == FUNCTION)
	{
		receiveFunction(commNodes, &function);
		//printFunction(function);
		//TODO provisorio
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
		if(function.countPreceders > 0) free( function.preceders );
		if(function.countFollowers > 0) free( function.followers );
		if(function.countProbabilisticBranch > 0) free(function.probabilisticBranch );
	}
	else if (status.MPI_TAG == COMBI)
	{
		receiveCombi(commNodes, &combi);
		//printCombi(combi);
		combiNode(commNodes, &combi, mpiProcesses);
		if(combi.countPreceders > 0) free( combi.preceders);
		if(combi.countFollowers > 0) free(combi.followers);
		if(combi.countProbabilisticBranch > 0) free(combi.probabilisticBranch);
	}
	
}


void receiveQueue(const MPI_Comm commNodes,Queue *queue)
{
	MPI_Recv3(queue, sizeof(Queue), MPI_BYTE, 0, QUEUE , commNodes, MPI_STATUS_IGNORE);
	if ((*queue).countPreceders>0) {
		(*queue).preceders = (int *) malloc( (*queue).countPreceders *sizeof(int));
		MPI_Recv12((*queue).preceders, (*queue).countPreceders, MPI_INT, 0, QUEUE , commNodes, MPI_STATUS_IGNORE);
	}
	if ((*queue).countFollowers>0) {
		(*queue).followers = (int *) malloc( (*queue).countFollowers *sizeof(int));
		MPI_Recv12((*queue).followers, (*queue).countFollowers, MPI_INT, 0, QUEUE , commNodes, MPI_STATUS_IGNORE);
	}
}

void receiveCounter(const MPI_Comm commNodes, Counter *counter)
{
	MPI_Recv3(counter, sizeof(Counter), MPI_BYTE, 0, COUNTER , commNodes, MPI_STATUS_IGNORE);
	if ((*counter).countPreceders>0) {
		(*counter).preceders = (int *) malloc( (*counter).countPreceders *sizeof(int));
		MPI_Recv12((*counter).preceders, (*counter).countPreceders, MPI_INT, 0, COUNTER , commNodes, MPI_STATUS_IGNORE);
	}
	if ((*counter).countFollowers>0) {
		(*counter).followers = (int *) malloc( (*counter).countFollowers *sizeof(int));
		MPI_Recv12((*counter).followers, (*counter).countFollowers, MPI_INT, 0, COUNTER , commNodes, MPI_STATUS_IGNORE);
	}
}

void receiveFunction(const MPI_Comm commNodes, Function *function)
{
	MPI_Recv3(function, sizeof(Function), MPI_BYTE, 0, FUNCTION , commNodes, MPI_STATUS_IGNORE);
	if ((*function).countPreceders>0) {
		(*function).preceders = (int *) malloc( (*function).countPreceders *sizeof(int));
		MPI_Recv12((*function).preceders, (*function).countPreceders, MPI_INT, 0, FUNCTION , commNodes, MPI_STATUS_IGNORE);
	}
	if ((*function).countFollowers>0) {
		(*function).followers = (int *) malloc( (*function).countFollowers *sizeof(int));
		MPI_Recv12((*function).followers, (*function).countFollowers, MPI_INT, 0, FUNCTION , commNodes, MPI_STATUS_IGNORE);
	}
	if ((*function).countProbabilisticBranch>0) {
		(*function).probabilisticBranch = (double *) malloc( (*function).countProbabilisticBranch *sizeof(double));
		MPI_Recv((*function).probabilisticBranch, (*function).countProbabilisticBranch, MPI_DOUBLE, 0, FUNCTION , commNodes, MPI_STATUS_IGNORE);
	}
}

void receiveNormal(const MPI_Comm commNodes, Normal *normal)
{
	MPI_Recv3(normal, sizeof(Normal), MPI_BYTE, 0, NORMAL , commNodes, MPI_STATUS_IGNORE);
	if ((*normal).countPreceders>0) {
		(*normal).preceders = (int *) malloc( (*normal).countPreceders *sizeof(int));
		MPI_Recv12((*normal).preceders, (*normal).countPreceders, MPI_INT, 0, NORMAL , commNodes, MPI_STATUS_IGNORE);
	}
	if ((*normal).countFollowers>0) {
		(*normal).followers = (int *) malloc( (*normal).countFollowers *sizeof(int));
		MPI_Recv12((*normal).followers, (*normal).countFollowers, MPI_INT, 0, NORMAL , commNodes, MPI_STATUS_IGNORE);
	}
	if ((*normal).countProbabilisticBranch>0) {
		(*normal).probabilisticBranch = (double *) malloc( (*normal).countProbabilisticBranch *sizeof(double));
		MPI_Recv((*normal).probabilisticBranch, (*normal).countProbabilisticBranch, MPI_DOUBLE, 0, NORMAL , commNodes, MPI_STATUS_IGNORE);
	}
}

void receiveCombi(const MPI_Comm commNodes, Combi *combi)
{
	MPI_Recv3(combi, sizeof(Combi), MPI_BYTE, 0, COMBI , commNodes, MPI_STATUS_IGNORE);
	if ((*combi).countPreceders>0) {
		(*combi).preceders = (int *) malloc( (*combi).countPreceders *sizeof(int));
		MPI_Recv12((*combi).preceders, (*combi).countPreceders, MPI_INT, 0, COMBI , commNodes, MPI_STATUS_IGNORE);
	}
	if ((*combi).countFollowers>0) {
		(*combi).followers = (int *) malloc( (*combi).countFollowers *sizeof(int));
		MPI_Recv12((*combi).followers, (*combi).countFollowers, MPI_INT, 0, COMBI , commNodes, MPI_STATUS_IGNORE);
	}
	if ((*combi).countProbabilisticBranch>0) {
		(*combi).probabilisticBranch = (double *) malloc( (*combi).countProbabilisticBranch *sizeof(double));
		MPI_Recv((*combi).probabilisticBranch, (*combi).countProbabilisticBranch, MPI_DOUBLE, 0, COMBI , commNodes, MPI_STATUS_IGNORE);
	}
}


void sendStruct(const MPI_Comm commNodes, Queue *queues,const int queuesCount, Counter *counters,const  int counterCount, Function *functions, const int functionCount, Normal *normals, const int normalCount, Combi *combis, const int combiCount)
{
	int i;

	// ENVIO DE 'QUEUES' (2 ENVIOS ADICIONALES PARA 'PRECEDERS' Y 'FOLLOWERS')
	for (i=0 ; i < queuesCount ; i++) //QUEUE
	{
		MPI_Send( &queues[i], sizeof(Queue),  MPI_BYTE, queues[i].idNode, QUEUE, commNodes);
		if ( queues[i].countPreceders > 0 )
			MPI_Send(queues[i].preceders, queues[i].countPreceders ,  MPI_INT, queues[i].idNode , QUEUE, commNodes);
		if ( queues[i].countFollowers > 0 )
			MPI_Send( queues[i].followers,  queues[i].countFollowers ,  MPI_INT, queues[i].idNode, QUEUE, commNodes);
	}

	// ENVIO DE NORMAL
	for (i=0 ; i < normalCount ; i++)
	{
		MPI_Send( &normals[i], sizeof(Normal),  MPI_BYTE, normals[i].idNode, NORMAL, commNodes);
		if ( normals[i].countPreceders > 0 )
			MPI_Send( normals[i].preceders, normals[i].countPreceders ,  MPI_INT, normals[i].idNode, NORMAL, commNodes);
		if ( normals[i].countFollowers > 0 )
			MPI_Send( normals[i].followers, normals[i].countFollowers ,  MPI_INT, normals[i].idNode, NORMAL, commNodes);
		if ( normals[i].countProbabilisticBranch > 0)
			MPI_Send( normals[i].probabilisticBranch, normals[i].countProbabilisticBranch ,  MPI_DOUBLE, normals[i].idNode, NORMAL, commNodes);
	}

	// ENVIO DE COUNTER
	for (i=0 ; i < counterCount ; i++)
	{
		MPI_Send(&counters[i], sizeof(Counter),  MPI_BYTE, counters[i].idNode, COUNTER, commNodes);
		if ( counters[i].countPreceders > 0 )
			MPI_Send( counters[i].preceders, counters[i].countPreceders ,  MPI_INT, counters[i].idNode, COUNTER, commNodes);
		if ( counters[i].countFollowers > 0 )
			MPI_Send( counters[i].followers, counters[i].countFollowers ,  MPI_INT,  counters[i].idNode, COUNTER, commNodes);
	}


	// ENVIO DE FUNCTION
	for (i=0 ; i < functionCount ; i++)
	{
		MPI_Send(&functions[i], sizeof(Function),  MPI_BYTE, functions[i].idNode, FUNCTION, commNodes);
		if ( functions[i].countPreceders > 0 )
			MPI_Send( functions[i].preceders, functions[i].countPreceders ,  MPI_INT, functions[i].idNode, FUNCTION, commNodes);
		if ( functions[i].countFollowers > 0 )
			MPI_Send( functions[i].followers, functions[i].countFollowers ,  MPI_INT, functions[i].idNode, FUNCTION, commNodes);
		if ( functions[i].countProbabilisticBranch > 0 )
			MPI_Send( functions[i].probabilisticBranch, functions[i].countProbabilisticBranch ,  MPI_DOUBLE, functions[i].idNode, FUNCTION, commNodes);
	}

	// ENVIO DE 'COMBIS' (3 ENVIOS ADICIONALES PARA 'PRECEDERS', 'FOLLOWERS' y 'PROBABILISTIC_BRANCH')
	for (i=0 ; i < combiCount ; i++)
	{
		MPI_Send( &combis[i], sizeof(Combi),  MPI_BYTE, combis[i].idNode, COMBI, commNodes);
		if (combis[i].countPreceders>0)
			MPI_Send( combis[i].preceders, combis[i].countPreceders ,  MPI_INT, combis[i].idNode, COMBI, commNodes);
		if ( combis[i].countFollowers > 0 )
			MPI_Send( combis[i].followers, combis[i].countFollowers ,  MPI_INT, combis[i].idNode, COMBI, commNodes);
		if ( combis[i].countProbabilisticBranch > 0)
			MPI_Send( combis[i].probabilisticBranch, combis[i].countProbabilisticBranch ,  MPI_DOUBLE, combis[i].idNode, COMBI, commNodes);
	}

}

void getQueues(const char *filenameJson , Queue **queues, int* queuesCount)
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

		if ((*queues)[i].countPreceders)(*queues)[i].preceders = (int *) malloc((*queues)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*queues)[i].countPreceders; j++)
			(*queues)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*queues)[i].countFollowers = json_array_get_count(arrayInternal);
		if((*queues)[i].countFollowers)(*queues)[i].followers = (int *) malloc((*queues)[i].countFollowers*sizeof(int));
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
		if((*counters)[i].countPreceders)(*counters)[i].preceders = (int *) malloc((*counters)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*counters)[i].countPreceders; j++)
			(*counters)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*counters)[i].countFollowers = json_array_get_count(arrayInternal);
		if((*counters)[i].countFollowers)(*counters)[i].followers = (int *) malloc((*counters)[i].countFollowers*sizeof(int));
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
		if((*functions)[i].countPreceders)(*functions)[i].preceders = (int *) malloc((*functions)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*functions)[i].countPreceders; j++)
			(*functions)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*functions)[i].countFollowers = json_array_get_count(arrayInternal);
		if((*functions)[i].countFollowers)(*functions)[i].followers = (int *) malloc((*functions)[i].countFollowers*sizeof(int));
		for (j = 0; j < (*functions)[i].countFollowers; j++)
			(*functions)[i].followers[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "probabilisticBranch");
		(*functions)[i].countProbabilisticBranch = json_array_get_count(arrayInternal);
		if((*functions)[i].countProbabilisticBranch)(*functions)[i].probabilisticBranch = (double *) malloc((*functions)[i].countProbabilisticBranch*sizeof(double));
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
		if((*normals)[i].countPreceders)(*normals)[i].preceders = (int *) malloc((*normals)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*normals)[i].countPreceders; j++)
			(*normals)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*normals)[i].countFollowers = json_array_get_count(arrayInternal);
		if((*normals)[i].countFollowers)(*normals)[i].followers = (int *) malloc((*normals)[i].countFollowers*sizeof(int));
		for (j = 0; j < (*normals)[i].countFollowers; j++)
			(*normals)[i].followers[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "probabilisticBranch");
		(*normals)[i].countProbabilisticBranch = json_array_get_count(arrayInternal);
		if((*normals)[i].countProbabilisticBranch)(*normals)[i].probabilisticBranch = (double *) malloc((*normals)[i].countProbabilisticBranch*sizeof(double));
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
		if((*combis)[i].countPreceders)(*combis)[i].preceders = (int *) malloc((*combis)[i].countPreceders*sizeof(int));
		for (j = 0; j < (*combis)[i].countPreceders; j++)
			(*combis)[i].preceders[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "followers");
		(*combis)[i].countFollowers = json_array_get_count(arrayInternal);
		if((*combis)[i].countFollowers)(*combis)[i].followers = (int *) malloc((*combis)[i].countFollowers*sizeof(int));
		for (j = 0; j < (*combis)[i].countFollowers; j++)
			(*combis)[i].followers[j]=json_array_get_number(arrayInternal,j);

		arrayInternal = json_object_dotget_array(objectInArray, "probabilisticBranch");
		(*combis)[i].countProbabilisticBranch = json_array_get_count(arrayInternal);
		if((*combis)[i].countProbabilisticBranch)(*combis)[i].probabilisticBranch = (double *) malloc((*combis)[i].countProbabilisticBranch*sizeof(double));
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

void printFunction(Function function){
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

void printNormal(Normal normal){
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

