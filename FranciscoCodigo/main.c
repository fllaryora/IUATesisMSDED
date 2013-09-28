/*
 * BotQueue and all modules are free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
 
#include "ourMPI.h"
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "scheduler.h"
#include "raffler.h"
#include "printer.h"
#include "genericNode.h"
#include "jsonHelper.h"

void logError(int error_code, int my_rank);
void master(int mpiProcesses, MPI_Comm commNodes);
void createCommunicator( MPI_Comm* commNodes, MPI_Group* groupNodes, MPI_Group* groupWorld, int** processRank, int mpiProcesses, int idNodo );

int main(int argc, char **argv){
	int idNodo; int idNodoInterno;  int mpiProcesses; 
	int* processRank = NULL; MPI_Group groupWorld; MPI_Group groupNodes; MPI_Comm commNodes;
	int jsonResult;
	/* Inicio de zona MPI */
	MPI_Init(&argc, &argv);
	/* Busco mi nodo Id y cantidad*/
	MPI_Comm_rank(MPI_COMM_WORLD, &idNodo);
	MPI_Comm_size(MPI_COMM_WORLD, &mpiProcesses);
	/*voy a armar comunicadores*/
	createCommunicator( &commNodes, &groupNodes, &groupWorld, &processRank, mpiProcesses, idNodo );
	MPI_Comm_rank( commNodes, &idNodoInterno);
	printf("nuevo rank %d => %d\n", idNodo, idNodoInterno);
	if ( idNodo == MASTER_ID ) {
		master(mpiProcesses, commNodes);
	} else {
		MPI_Bcast_JSON( &jsonResult );
		if ( jsonResult == GOOD_JSON ) {
			if ( idNodo == RAFFLER_ID ) {
				raffler();
			} else {
				if (idNodo == PRINTER_ID) {
					printer();
				} else {
					MPI_Barrier( commNodes );
					genericNode(idNodo);
				}
			}
		} else {
			printf("Master node has sent BAD_JSON by broadcast\n");
		}		
	}
	/* FIN de zona de MPI */
	if(processRank != NULL)free(processRank);
	MPI_Finalize();
	return 0;
}

void master(int mpiProcesses, MPI_Comm commNodes){
	int jsonResult;
	if ( validateJsonInput() == JSON_APPROVED ) {			
		if ( getNodesAmount() + MASTER_RAFFLER_PRINTER == mpiProcesses ) {
			putNodesInMem();
			//broadcast TAG JSON BUENO
			jsonResult = GOOD_JSON;
			MPI_Bcast_JSON(&jsonResult);
			//enviar lo combisIds al raffler
			int* seedAndCombis = getCombiIds( );
			SendCombisToRaffler( &seedAndCombis[1] ,  seedAndCombis[0] );
			free(seedAndCombis);
			scheduler();
			SendLiveLockToRaffler();
			MPI_Barrier( commNodes );
			//int saraza = 999;
			//MPI_Bcast( &saraza, 1 , MPI_INT, 0 ,commNodes);
			//SendLiveLockToPrinter();
			/* Shut down MPI */
			return;
		}
	} 
	//Broadcast Json malo
	jsonResult = BAD_JSON;
	MPI_Bcast_JSON( &jsonResult );
}
	
void logError(int error_code, int my_rank){
	if (error_code != MPI_SUCCESS) {
		char error_string[MPI_MAX_ERROR_STRING];
		int length_of_error_string, error_class;
		getErrorCalss(error_code, &error_class);
		getErrorString(error_class, error_string, &length_of_error_string);
		printf("%3d: %s     \n", my_rank, error_string);
		getErrorString(error_code, error_string, &length_of_error_string);
		printf("%3d: %s     \n", my_rank, error_string);
		abortAllProcess(error_code);
	}
}

void createCommunicator( MPI_Comm* commNodes, MPI_Group* groupNodes, MPI_Group* groupWorld, int** processRank, int mpiProcesses, int idNodo ){
	int error_code;
	if(mpiProcesses < 2){return;}
	int* myProcessRank = (int*) malloc((mpiProcesses - RAFFLER_PRINTER)*sizeof(int));
	myProcessRank[0] = MASTER_ID;
	for(int i = FIST_NODE_ID; i < mpiProcesses ; i++){
		myProcessRank[i - RAFFLER_PRINTER] = i;
	}
	setLoger();
	error_code = GetCommWorldHandle(groupWorld);
	logError( error_code, idNodo);
	if(idNodo == RAFFLER_ID || idNodo == PRINTER_ID ) {
		myProcessRank[0] = RAFFLER_ID;
		myProcessRank[1] = PRINTER_ID;
		error_code = CreateGroupByIds(*groupWorld, RAFFLER_PRINTER, myProcessRank, groupNodes );
		logError( error_code, idNodo);
	} else{
		error_code = CreateGroupByIds(*groupWorld,(mpiProcesses - RAFFLER_PRINTER), myProcessRank, groupNodes );
		logError( error_code, idNodo);
	}
	error_code = CreateCommByGroup(*groupNodes, commNodes);
	logError( error_code, idNodo);
	unsetLoger();
	*processRank = myProcessRank;
}
