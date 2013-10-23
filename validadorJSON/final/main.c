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

	int idNodo, tag=2, mpiProcesses; 

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
		queues[0].idNode = 1;
	   	queues[0].resource = 2;
		queues[0].fixedCost = 9.8;
		queues[0].variableCost = 7.5;
		queues[0].preceders = (int *) malloc(3*sizeof(int));
		queues[0].preceders[0]=2;
		queues[0].preceders[1]=2;
		queues[0].preceders[2]=3;
		queues[0].followers = (int *) malloc(4*sizeof(int));
		queues[0].followers[0]=3;
		queues[0].followers[1]=7;
		queues[0].followers[2]=10;
		queues[0].followers[3]=12;

		queues[1].idNode = 2;
	   	queues[1].resource = 0;
		queues[1].fixedCost = 0.8;
		queues[1].variableCost = 0.5;
		queues[1].preceders = (int *) malloc(2*sizeof(int));
		queues[1].preceders[0]=1;
		queues[1].preceders[1]=2;
		queues[1].followers = (int *) malloc(2*sizeof(int));
		queues[1].followers[0]=1;
		queues[1].followers[1]=6;

		MPI_Send(&queues[0], sizeof(Queue),  MPI_BYTE, 1, tag, MPI_COMM_WORLD);
		MPI_Send(&queues[1], sizeof(Queue),  MPI_BYTE, 2, tag, MPI_COMM_WORLD);
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
		MPI_Recv(&queue, sizeof(Queue), MPI_BYTE, MPI_ANY_SOURCE, tag , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("idNode: %d\n", queue.idNode);
		printf("resource: %d\n", queue.resource);
		printf("fixedCost: %.4f\n", queue.fixedCost);
		printf("variableCost: %.4f\n", queue.variableCost);
		/*printf("preceders: %d %d %d\n\n", queues[0].preceders[0], queues[0].preceders[1],queues[0].preceders[2]);*/

	}

	MPI_Finalize();
	return 0;
}
