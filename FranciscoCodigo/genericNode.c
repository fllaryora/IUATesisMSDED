#include "ourMPI.h"
#include "genericNode.h"
#include "jsonHelper.h"
#include <stdio.h>
void genericNode(int myIdNodo){
	int slaveNodes;
	MPI_Group groupWorld;
	MPI_Group groupNodes;
	MPI_Comm commNodes;
	int* processRank;
	
	/* Find out how many processes there are in the default communicator */
	MPI_Comm_size(MPI_COMM_WORLD, &slaveNodes);
	// make a list of processes in the new communicator
	processRank = (int*) malloc((slaveNodes - RAFFLER_PRINTER)*sizeof(int));
	processRank[0] = MASTER_ID;
	for(int i = FIST_NODE_ID; i < slaveNodes ; i++){
		printf(" %d = %d\n", i - RAFFLER_PRINTER ,i);
		processRank[i - RAFFLER_PRINTER] = i;
	}
	printf("Los nuevos rank son\n");
	for(int i = 0 ; i < (slaveNodes - RAFFLER_PRINTER);i++)
		printf("%d ",processRank[i]);
	printf("\n");
	
	//get the group under MPI_COMM_WORLD
	MPI_Comm_group(MPI_COMM_WORLD, groupWorld);
	// create the new group
	MPI_Group_incl(groupWorld, (slaveNodes - RAFFLER_PRINTER), processRank, &groupNodes);
	// create the new communicator
	MPI_Comm_create(MPI_COMM_WORLD, groupNodes, &commNodes);
	//---------------------------------------------------
	
	
	
	
	
	
	getNodeById( myIdNodo );
	printf("Hello from slave\n");
	
	
	
	
	
	
	//----------------------
	MPI_Comm_free(&commNodes);
	MPI_Group_free(&groupNodes);
	free(processRank);
}
