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
void master(const int mpiProcesses, const MPI_Comm commNodes,const char *filenameJson );
void createCommunicator( MPI_Comm* commNodes, MPI_Group* groupNodes, MPI_Group* groupWorld, int** processRank, int mpiProcesses, int idNodo );

int main(int argc, char **argv){

	const char *filenameJson   = "archivos/modelo.json";
	
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
	switch( idNodo ){
		case MASTER_ID:
			master(mpiProcesses, commNodes,filenameJson);
			break;
		case RAFFLER_ID:
			MPI_Bcast_JSON( &jsonResult, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
			if ( jsonResult == GOOD_JSON ) {
				raffler();
			}else {
				printf("Master node has sent BAD_JSON by broadcast\n");
			}
		break;
		case PRINTER_ID:
			MPI_Bcast_JSON( &jsonResult, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
			if ( jsonResult == GOOD_JSON ) {
				//printer();
			}else {
				printf("Master node has sent BAD_JSON by broadcast\n");
			}
			break;
		default :
			MPI_Bcast_JSON( &jsonResult, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
			if ( jsonResult == GOOD_JSON ) {
				genericNode(idNodo, idNodoInterno, commNodes);
			}else {
				//printf("Master node has sent BAD_JSON by broadcast\n");
			}
			break;
	}
	/* FIN de zona de MPI */
	if(processRank != NULL)free(processRank);
	MPI_Finalize();
	return 0;
}

void master(const int mpiProcesses, const MPI_Comm commNodes ,const char *filenameJson ){
	int jsonResult;
	if ( validateJsonInput(filenameJson) == VALIDATION_PASS ) {		
		if ( getNodesAmount(filenameJson) + MASTER_RAFFLER_PRINTER == mpiProcesses ) {
			sendStructToNodes(filenameJson);
			//broadcast TAG JSON BUENO
			jsonResult = GOOD_JSON;
			MPI_Bcast_JSON( &jsonResult, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
			//enviar lo combisIds al raffler
			int* seedAndCombis = getCombiIds( filenameJson);
			MPI_Send( &seedAndCombis[1] ,  seedAndCombis[0]  , MPI_INT , RAFFLER_ID , SEED_AND_COMBI_LIST , MPI_COMM_WORLD);
			free(seedAndCombis);
			int* targetCounter = getTargets( filenameJson);

			//TODO watchdog = 0 para que lo haga una vez  cambiar por getWatchdog( filenameJson); despues
			scheduler( 0, commNodes , &targetCounter[1] , mpiProcesses, targetCounter[0]);
			free(targetCounter);

			/* Shut down MPI */
			return;
		}
		else {
			printf("Error en la cantidad de nodos contra procesos\n");
		}
	} 
	//Broadcast Json malo
	jsonResult = BAD_JSON;
	MPI_Bcast_JSON( &jsonResult, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
}
	
void logError(int error_code, int my_rank){
	if (error_code != MPI_SUCCESS) {
		char error_string[MPI_MAX_ERROR_STRING];
		int length_of_error_string, error_class;
		MPI_Error_class(error_code, &error_class);
		MPI_Error_string(error_class, error_string, &length_of_error_string);
		printf("%3d: %s     \n", my_rank, error_string);
		MPI_Error_string(error_code, error_string, &length_of_error_string);
		printf("%3d: %s     \n", my_rank, error_string);
		MPI_Abort(MPI_COMM_WORLD, error_code);
	}
}

void createCommunicator( MPI_Comm* commNodes, MPI_Group* groupNodes, MPI_Group* groupWorld, int** processRank, int mpiProcesses, int idNodo ){
	int error_code;
	if(mpiProcesses < 2){return;}
	int* myProcessRank = NULL;
	MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
	error_code =  MPI_Comm_group(MPI_COMM_WORLD, groupWorld);
	logError( error_code, idNodo);
	if(idNodo == RAFFLER_ID || idNodo == PRINTER_ID ) {
		myProcessRank = (int*) malloc( RAFFLER_PRINTER*sizeof(int));
		myProcessRank[0] = RAFFLER_ID;
		myProcessRank[1] = PRINTER_ID;
		error_code = MPI_Group_incl(*groupWorld, RAFFLER_PRINTER, myProcessRank, groupNodes );
		logError( error_code, idNodo);
	} else {
		myProcessRank = (int*) malloc((mpiProcesses - RAFFLER_PRINTER)*sizeof(int));
		myProcessRank[0] = MASTER_ID;
		for(int i = FIST_NODE_ID; i < mpiProcesses ; i++){
			myProcessRank[i - RAFFLER_PRINTER] = i;
		}
		error_code = MPI_Group_incl(*groupWorld,(mpiProcesses - RAFFLER_PRINTER), myProcessRank, groupNodes );
		logError( error_code, idNodo);
	}
	error_code =  MPI_Comm_create(MPI_COMM_WORLD, *groupNodes, commNodes);
	logError( error_code, idNodo);
	 MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_ARE_FATAL);
	*processRank = myProcessRank;
}
