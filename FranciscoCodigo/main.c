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

int main(int argc, char **argv){
	int idNodo;
	int mpiProcesses;
	int jsonResult;
	
	/* Inicio de zona MPI */
	MPI_Init(&argc, &argv);
	
	/* Busco mi nodo Id */
	MPI_Comm_rank(MPI_COMM_WORLD, &idNodo);
	
	if ( idNodo == MASTER_ID ) {
		if ( validateJsonInput() == JSON_APPROVED ) {
			MPI_Comm_size(MPI_COMM_WORLD, &mpiProcesses);
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
	MPI_Finalize();
	return 0;
}
