#include "validador.h"
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

int main(int argc, char **argv){
	int rta;
	const char *filenameJson   = "archivos/modelo.json";
	const char *filenameSchema = "archivos/schema.json";

	int idNodo, tag=2,tipo, mpiProcesses; 
	int receiverCount,currentTag;

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
	
		Queue *queues = (Queue *) malloc(2*sizeof(Queue));
		queues[0].idNode = 44;
	   	queues[0].resource = 2;
		queues[0].fixedCost = 9.8;
		queues[0].variableCost = 7.5;
		queues[0].countPreceders = 2;
		queues[0].preceders = (int *) malloc(2*sizeof(int));
		queues[0].preceders[0]=2;
		queues[0].preceders[1]=3;
		queues[0].countFollowers = 3;
		queues[0].followers = (int *) malloc(3*sizeof(int));
		queues[0].followers[0]=7;
		queues[0].followers[1]=10;
		queues[0].followers[2]=12;

		queues[1].idNode = 2;
	   	queues[1].resource = 0;
		queues[1].fixedCost = 0.8;
		queues[1].variableCost = 0.5;
		queues[1].countPreceders = 1;
		queues[1].preceders = (int *) malloc(2*sizeof(int));
		queues[1].preceders[0]=99;
		queues[1].countFollowers = 1;
		queues[1].followers = (int *) malloc(2*sizeof(int));
		queues[1].followers[0]=6;

		tag = 1; /*Queue*/
		tipo = 1; /*Queue*/

		/*MPI_Send(&tipo, sizeof(int),  MPI_INT, 1, tag, MPI_COMM_WORLD);*/
		MPI_Send(&queues[0], sizeof(Queue),  MPI_BYTE, 1, tag, MPI_COMM_WORLD);
		if (queues[0].countPreceders>0)
			MPI_Send(queues[0].preceders, queues[0].countPreceders ,  MPI_INT, 1, tag, MPI_COMM_WORLD);

		/*MPI_Send(&tipo, sizeof(int),  MPI_INT, 2, tag, MPI_COMM_WORLD);*/
		MPI_Send(&queues[1], sizeof(Queue),  MPI_BYTE, 2, tag, MPI_COMM_WORLD);
		if (queues[1].countPreceders>0)
			MPI_Send(queues[1].preceders, queues[1].countPreceders ,  MPI_INT, 2, tag, MPI_COMM_WORLD);
		/*printf("idNode: %d\n", queues[0].idNode);
		printf("resource: %d\n", queues[0].resource);
		printf("fixedCost: %.4f\n", queues[0].fixedCost);
		printf("variableCost: %.4f\n", queues[0].variableCost);
		printf("preceders: %d %d %d\n\n", queues[0].preceders[0], queues[0].preceders[1],queues[0].preceders[2]);

		printf("idNode: %d\n", queues[1].idNode);
		printf("resource: %d\n", queues[1].resource);
		printf("fixedCost: %.4f\n", queues[1].fixedCost);
		printf("variableCost: %.4f\n\n", queues[1].variableCost);

		printf("sizeof: %d %d\n", sizeof(queues[0]), sizeof(queues[1]));*/

	} else {
		Queue queue;
		Counter counter;

		MPI_Probe( 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		//MPI_Recv(&tipo, sizeof(int), MPI_INT, 0, MPI_ANY_TAG , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if (status.MPI_TAG == 1) // Queue //TODO: USAR ENUM
		{
			tag = 1;
			MPI_Recv(&queue, sizeof(Queue), MPI_BYTE, 0, tag , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("idNode: %d\n", queue.idNode);
			printf("resource: %d\n", queue.resource);
			printf("fixedCost: %.4f\n", queue.fixedCost);
			printf("variableCost: %.4f\n", queue.variableCost);
			printf("countPreceders: %d\n", queue.countPreceders);
			printf("countFollowers: %d\n", queue.countFollowers);

			queue.preceders = (int *) malloc( queue.countPreceders *sizeof(int));

			if (queue.countPreceders>0)
				MPI_Recv(queue.preceders, queue.countPreceders, MPI_INT, 0, tag , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			/*if (queue.countFollowers>0)
				MPI_Recv(&queue.followers, queue.countFollowers, MPI_INT, 0, tag , MPI_COMM_WORLD, MPI_STATUS_IGNORE);*/

			printf("preceders: %d\n", queue.preceders[0]);
			/*printf("followers: %d\n", queue.followers[0]);

			/*MPI_Get_count(&infoComm, MPI_INT, &receiverCount)
			if( receiverCount == 0 ) receiverCount = 1;
			printf("receiverCount = %d\n", receiverCount);
			if(receiverCount){
				bufferReceiver = (int*)malloc(sizeof(int) * receiverCount);
			}
			MPI_Recv(bufferReceiver, receiverCount, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE)*/
			/*printf("preceders: %d %d %d\n\n", queues[0].preceders[0], queues[0].preceders[1],queues[0].preceders[2]);*/
		}
		else if (status.MPI_TAG == 2) //Counter
		{
			
		}

	}

	MPI_Finalize();
	return 0;
}
