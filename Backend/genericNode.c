#include "ourMPI.h"
#include "main.h"
#include "genericNode.h"
#include "jsonHelper.h"
#include <stdio.h>
#include <stdlib.h>

void genericNode(int myIdNodo){
	
	printf("Hello from slave\n");
	MPI_Status status;
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
		else if (status.MPI_TAG == COUNTER)
	{
		receiveCounter(&counter);
		//printCounter(counter);
	}
	else if (status.MPI_TAG == NORMAL)
	{
		receiveNormal(&normal);
		printNormal(normal);
	}
	else if (status.MPI_TAG == FUNCTION)
	{
		receiveFunction(&function);
		//printFunction(function);
	}
	else if (status.MPI_TAG == COMBI)
	{
		receiveCombi(&combi);
		//printCombi(combi);
	}

}

void receiveQueue(Queue *queue)
{
	MPI_Recv3(queue, sizeof(Queue), MPI_BYTE, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if ((*queue).countPreceders>0) {
		(*queue).preceders = (int *) malloc( (*queue).countPreceders *sizeof(int));
		MPI_Recv12((*queue).preceders, (*queue).countPreceders, MPI_INT, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*queue).countFollowers>0) {
		(*queue).followers = (int *) malloc( (*queue).countFollowers *sizeof(int));
		MPI_Recv12((*queue).followers, (*queue).countFollowers, MPI_INT, 0, QUEUE , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

void receiveCounter(Counter *counter)
{
	MPI_Recv3(counter, sizeof(Counter), MPI_BYTE, 0, COUNTER , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if ((*counter).countPreceders>0) {
		(*counter).preceders = (int *) malloc( (*counter).countPreceders *sizeof(int));
		MPI_Recv12((*counter).preceders, (*counter).countPreceders, MPI_INT, 0, COUNTER , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*counter).countFollowers>0) {
		(*counter).followers = (int *) malloc( (*counter).countFollowers *sizeof(int));
		MPI_Recv12((*counter).followers, (*counter).countFollowers, MPI_INT, 0, COUNTER , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

void receiveFunction(Function *function)
{
	MPI_Recv3(function, sizeof(Function), MPI_BYTE, 0, FUNCTION , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if ((*function).countPreceders>0) {
		(*function).preceders = (int *) malloc( (*function).countPreceders *sizeof(int));
		MPI_Recv12((*function).preceders, (*function).countPreceders, MPI_INT, 0, FUNCTION , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*function).countFollowers>0) {
		(*function).followers = (int *) malloc( (*function).countFollowers *sizeof(int));
		MPI_Recv12((*function).followers, (*function).countFollowers, MPI_INT, 0, FUNCTION , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*function).countProbabilisticBranch>0) {
		(*function).probabilisticBranch = (double *) malloc( (*function).countProbabilisticBranch *sizeof(double));
		MPI_Recv((*function).probabilisticBranch, (*function).countProbabilisticBranch, MPI_DOUBLE, 0, FUNCTION , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

void receiveNormal(Normal *normal)
{
	MPI_Recv3(normal, sizeof(Normal), MPI_BYTE, 0, NORMAL , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if ((*normal).countPreceders>0) {
		(*normal).preceders = (int *) malloc( (*normal).countPreceders *sizeof(int));
		MPI_Recv12((*normal).preceders, (*normal).countPreceders, MPI_INT, 0, NORMAL , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*normal).countFollowers>0) {
		(*normal).followers = (int *) malloc( (*normal).countFollowers *sizeof(int));
		MPI_Recv12((*normal).followers, (*normal).countFollowers, MPI_INT, 0, NORMAL , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*normal).countProbabilisticBranch>0) {
		(*normal).probabilisticBranch = (double *) malloc( (*normal).countProbabilisticBranch *sizeof(double));
		MPI_Recv((*normal).probabilisticBranch, (*normal).countProbabilisticBranch, MPI_DOUBLE, 0, NORMAL , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

void receiveCombi(Combi *combi)
{
	MPI_Recv3(combi, sizeof(Combi), MPI_BYTE, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if ((*combi).countPreceders>0) {
		(*combi).preceders = (int *) malloc( (*combi).countPreceders *sizeof(int));
		MPI_Recv12((*combi).preceders, (*combi).countPreceders, MPI_INT, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*combi).countFollowers>0) {
		(*combi).followers = (int *) malloc( (*combi).countFollowers *sizeof(int));
		MPI_Recv12((*combi).followers, (*combi).countFollowers, MPI_INT, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	if ((*combi).countProbabilisticBranch>0) {
		(*combi).probabilisticBranch = (double *) malloc( (*combi).countProbabilisticBranch *sizeof(double));
		MPI_Recv((*combi).probabilisticBranch, (*combi).countProbabilisticBranch, MPI_DOUBLE, 0, COMBI , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}
