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

		//TODO: LEER JSON Y RETORNAR LISTA DE ESTRUCTURAS Y CANTIDAD
		getQueues(&queues, &queuesCount);
		getCounters(&counters, &counterCount);
		getFunctions(&functions, &functionCount);
		getNormals(&normals, &normalCount);
		getCombis(&combis, &combiCount);


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
			MPI_Recv(&queue, sizeof(Queue), MPI_BYTE, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("countPreceders: %d\n", queue.countPreceders);
			printf("countFollowers: %d\n", queue.countFollowers);
			if (queue.countPreceders>0) {
				queue.preceders = (int *) malloc( queue.countPreceders *sizeof(int));
				MPI_Recv(queue.preceders, queue.countPreceders, MPI_INT, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			if (queue.countFollowers>0) {
				queue.followers = (int *) malloc( queue.countFollowers *sizeof(int));
				MPI_Recv(queue.followers, queue.countFollowers, MPI_INT, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}

			 //printQueue(queue);
		}
		else if (status.MPI_TAG == COMBI)
		{
			MPI_Recv(&combi, sizeof(Combi), MPI_BYTE, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//printf("countPreceders: %d\n", combi.countPreceders);
			//printf("countFollowers: %d\n", combi.countFollowers);
			//printf("countProbabilisticBranch: %d\n", combi.countProbabilisticBranch);
			if (combi.countPreceders>0) {
				combi.preceders = (int *) malloc( combi.countPreceders *sizeof(int));
				MPI_Recv(combi.preceders, combi.countPreceders, MPI_INT, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			if (combi.countFollowers>0) {
				combi.followers = (int *) malloc( combi.countFollowers *sizeof(int));
				MPI_Recv(combi.followers, combi.countFollowers, MPI_INT, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			if (combi.countProbabilisticBranch>0) {
				combi.probabilisticBranch = (double *) malloc( combi.countProbabilisticBranch *sizeof(double));
				MPI_Recv(combi.probabilisticBranch, combi.countProbabilisticBranch, MPI_DOUBLE, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}

			//printCombi(combi);
		}

	}

	MPI_Finalize();
	return 0;
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

/*pasar a otro .c*/
int getCountNodes (int **arrayCountNodes)
{
	*arrayCountNodes = (int*)malloc(5*sizeof(int));
	(*arrayCountNodes)[0] = 2; // QUEUE
	(*arrayCountNodes)[1] = 0; // NORMAL
	(*arrayCountNodes)[2] = 0; // COUNTER
	(*arrayCountNodes)[3] = 0; // FUNCTION
	(*arrayCountNodes)[4] = 1; // COMBI
	return 0;
}

void getQueues(Queue **queues, int *queuesCount)
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

void getCounters(Counter **normals, int *counterCount)
{
	*counterCount = 0;
}

void getFunctions(Function **functions, int *functionCount)
{
	*functionCount = 0;
}

void getNormals(Normal **normals, int *normalCount)
{
	*normalCount = 0;
}

void getCombis(Combi **combis, int *combiCount)
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
