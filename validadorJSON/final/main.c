#include "validador.h"
#include "main.h"
#include <mpi.h>
/* #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wjelement.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>*/

#include <string.h>

#define STREQ(A, B) (A && B ? strcmp(A, B) == 0 : 0)

int main(int argc, char **argv)
{
	const char *filenameJson   = "archivos/modelo.json";
	const char *filenameSchema = "archivos/schema.json";

	int i,j;
	int idNodo, mpiProcesses; 

	MPI_Status infoComm;
	MPI_Status result;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &idNodo);
	MPI_Comm_size(MPI_COMM_WORLD, &mpiProcesses);

	if (validateSchema(filenameJson , filenameSchema) != VALIDATION_PASS)
	{
		printf("Validacion por Schema Fallido\n");
		return 0;
	}

	if (validateJsonImput(filenameJson) != VALIDATION_PASS)
	{
		printf("Validacion datos Json Fallido\n");
		return 0;
	}

	if ( idNodo == 0 ) {
	
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

	} else if ( idNodo >= FIST_NODE_ID ) {

		Queue queue;
		Counter counter;
		Function function;
		Normal normal;
		Combi combi;

		// RECIBO ESTRUCTURA PARTICULAR, DEJO DE SER NODO_GENERICO
		MPI_Probe( 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if (status.MPI_TAG == QUEUE)
		{
			receiveQueue(&queue);
			//printQueue(queue);
		}
		else if (status.MPI_TAG == COMBI)
		{
			receiveCombi(&combi);
			printCombi(combi);
		}

	}

	MPI_Finalize();
	return 0;
}

void sendStruct(Queue **queues, int *queuesCount,Counter **counters, int *counterCount,Function **functions, int *functionCount,Normal **normals, int *normalCount,Combi **combis, int *combiCount)
{
	int i,j;

	// ENVIO DE 'QUEUES' (2 ENVIOS ADICIONALES PARA 'PRECEDERS' Y 'FOLLOWERS')
	for (i=0,j=0 ; i < *queuesCount ; i++,j++) //QUEUE
	{
		MPI_Send(&(*queues)[i], sizeof(Queue),  MPI_BYTE, j+MASTER_RAFFLER_PRINTER, QUEUE, MPI_COMM_WORLD);
		if ((*queues)[i].countPreceders>0)
			MPI_Send((*queues)[i].preceders, (*queues)[i].countPreceders ,  MPI_INT, j+MASTER_RAFFLER_PRINTER, QUEUE, MPI_COMM_WORLD);
		if ((*queues)[i].countFollowers>0)
			MPI_Send((*queues)[i].followers, (*queues)[i].countFollowers ,  MPI_INT, j+MASTER_RAFFLER_PRINTER, QUEUE, MPI_COMM_WORLD);
	}

	// ENVIO DE NORMAL
	// ENVIO DE COUNTER
	// ENVIO DE FUNCTION

	// ENVIO DE 'COMBIS' (3 ENVIOS ADICIONALES PARA 'PRECEDERS', 'FOLLOWERS' y 'PROBABILISTIC_BRANCH')
	for (i=0 ; i < *combiCount ; i++,j++)
	{
		MPI_Send(combis[i], sizeof(Combi),  MPI_BYTE, i+j+MASTER_RAFFLER_PRINTER, COMBI, MPI_COMM_WORLD);
		if ((*combis)[i].countPreceders>0)
			MPI_Send((*combis)[i].preceders, (*combis)[i].countPreceders ,  MPI_INT, j+MASTER_RAFFLER_PRINTER, COMBI, MPI_COMM_WORLD);
		if ((*combis)[i].countFollowers>0)
			MPI_Send((*combis)[i].followers, (*combis)[i].countFollowers ,  MPI_INT, j+MASTER_RAFFLER_PRINTER, COMBI, MPI_COMM_WORLD);
		if ((*combis)[i].countProbabilisticBranch>0)
			MPI_Send((*combis)[i].probabilisticBranch, (*combis)[i].countProbabilisticBranch ,  MPI_DOUBLE, j+MASTER_RAFFLER_PRINTER, COMBI, MPI_COMM_WORLD);
	}
}

void receiveQueue(Queue *queue)
{
	MPI_Recv(queue, sizeof(Queue), MPI_BYTE, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if ((*queue).countPreceders>0) {
		(*queue).preceders = (int *) malloc( (*queue).countPreceders *sizeof(int));
		MPI_Recv((*queue).preceders, (*queue).countPreceders, MPI_INT, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*queue).countFollowers>0) {
		(*queue).followers = (int *) malloc( (*queue).countFollowers *sizeof(int));
		MPI_Recv((*queue).followers, (*queue).countFollowers, MPI_INT, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

void receiveCounter(Counter *counter)
{

}

void receiveFunction(Function *function)
{

}

void receiveNormal(Normal *normal)
{

}

void receiveCombi(Combi *combi)
{
	MPI_Recv(combi, sizeof(Combi), MPI_BYTE, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if ((*combi).countPreceders>0) {
		(*combi).preceders = (int *) malloc( (*combi).countPreceders *sizeof(int));
		MPI_Recv((*combi).preceders, (*combi).countPreceders, MPI_INT, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*combi).countFollowers>0) {
		(*combi).followers = (int *) malloc( (*combi).countFollowers *sizeof(int));
		MPI_Recv((*combi).followers, (*combi).countFollowers, MPI_INT, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*combi).countProbabilisticBranch>0) {
		(*combi).probabilisticBranch = (double *) malloc( (*combi).countProbabilisticBranch *sizeof(double));
		MPI_Recv((*combi).probabilisticBranch, (*combi).countProbabilisticBranch, MPI_DOUBLE, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}
