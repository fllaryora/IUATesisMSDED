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

void logError(int error_code);

int main(int argc, char **argv){
	int idNodo; int idNodoInterno;
	MPI_Group groupWorld; MPI_Group groupNodes; MPI_Comm commNodes;
	int mpiProcesses; int* processRank = NULL;
	int jsonResult;
	int error_code;
	/* Inicio de zona MPI */
	MPI_Init(&argc, &argv);
	
	/* Busco mi nodo Id */
	MPI_Comm_rank(MPI_COMM_WORLD, &idNodo);
	
	if ( idNodo != RAFFLER_ID && idNodo != PRINTER_ID ) {
		/*voy a armar comunicadores*/
		MPI_Comm_size(MPI_COMM_WORLD, &mpiProcesses);
		processRank = (int*) malloc((mpiProcesses - RAFFLER_PRINTER)*sizeof(int));
		processRank[0] = MASTER_ID;
		for(int i = FIST_NODE_ID; i < mpiProcesses ; i++){
			processRank[i - RAFFLER_PRINTER] = i;
		}
		printf("Se genero el nuevo arreglo de ranks\n");
		
		MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
		
		error_code = GetCommWorldHandle(&groupWorld);
		logError( error_code);
		printf("Obtengo el hande del comm world\n");
		error_code = CreateGroupByIds(groupWorld,(mpiProcesses - RAFFLER_PRINTER), processRank, &groupNodes );
		logError( error_code);
		printf("Cree un grupo con los rangos\n");
		error_code = CreateCommByGroup(groupNodes, &commNodes);
		logError( error_code);
		printf("Cree un comunicador a partir del grupo\n");
		MPI_Comm_rank( commNodes, &idNodoInterno);
		printf("My bicycle = %d\n", idNodoInterno);
		//Test comunicator
		//MPI_Barrier( commNodes );
		printf("\n");

	}
	
	if ( idNodo == MASTER_ID ) {
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
				
				//SendLiveLockToPrinter();
				/* Shut down MPI */
				MPI_Finalize();
				return 0;
			}
		} 
		//Broadcast Json malo
		jsonResult = BAD_JSON;
		MPI_Bcast_JSON( &jsonResult );
	} else {
		MPI_Bcast_JSON( &jsonResult );
		if ( jsonResult == GOOD_JSON ) {
			if ( idNodo == RAFFLER_ID ) {
				raffler();
			} else {
				if (idNodo == PRINTER_ID) {
					printer();
				} else {
					genericNode(idNodo);
				}
			}
		} else {
			printf("Master node has sent BAD_JSON by broadcast\n");
		}
				
	}
	
	
	/* FIN de zona de MPI */
	if ( idNodo != RAFFLER_ID && idNodo != PRINTER_ID ) {
			if(processRank != NULL)free(processRank);
	}
	MPI_Finalize();
	return 0;
}

void logError(int error_code){
	if (error_code != MPI_SUCCESS) {

		char error_string[MPI_MAX_ERROR_STRING];
		int length_of_error_string, error_class;

		MPI_Error_class(error_code, &error_class);
		MPI_Error_string(error_class, error_string, &length_of_error_string);
		printf("%3d: %s\n", my_rank, error_string);
		MPI_Error_string(error_code, error_string, &length_of_error_string);
		printf("%3d: %s\n", my_rank, error_string);
		MPI_Abort(MPI_COMM_WORLD, error_code);
	}
}
