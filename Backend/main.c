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

//benchmark code
//#define USED_CLOCK CLOCK_MONOTONIC // CLOCK_MONOTONIC_RAW if available
//#define NANOS 1000000000LL

void logError(int error_code, int my_rank);
//benchmark code
//void master(const int mpiProcesses, const MPI_Comm commNodes,const char *filenameJson, int benckmarkCsv );
void master(const int mpiProcesses, const MPI_Comm commNodes,const char *filenameJson );
void createCommunicator( MPI_Comm* commNodes, MPI_Group* groupNodes, MPI_Group* groupWorld, int** processRank, int mpiProcesses, int idNodo );
//void putUnsigned(int fileDescriptor, const long long nro);

int main(int argc, char **argv){
	//benchmark code
	/*struct timespec startTime , endTime;
	long long runningTime = 0LL;
	long long start = 0LL;
	long long end = 0LL;
	int benckmarkCsv = 0;*/

	char* botqueueInputFile = getenv("BOTQUEUE_INPUT_FILE");
	if(botqueueInputFile == NULL ){
		botqueueInputFile = "/tmp/defaultInputJson.json";
		printf("Process Printer: can not find BOTQUEUE_INPUT_FILE, using default path: /tmp/defaultInputJson.json \n");
	}

	const char *filenameJson = botqueueInputFile;
	
	int idNodo; int idNodoInterno;  int mpiProcesses; 
	int* processRank = NULL; MPI_Group groupWorld; MPI_Group groupNodes; MPI_Comm commNodes;
	int jsonResult;
	/* Inicio de zona MPI */
	MPI_Init(&argc, &argv);
	/* Busco mi nodo Id y cantidad*/
	MPI_Comm_rank(MPI_COMM_WORLD, &idNodo);

	//benchmark code
	/*if(idNodo == MASTER_ID){
		benckmarkCsv = open ("/tmp/benchmark.csv" , O_WRONLY|O_CREAT|O_APPEND,00660);
		clock_gettime(USED_CLOCK, &startTime);
		start = startTime.tv_sec * NANOS + startTime.tv_nsec;
	}*/

	MPI_Comm_size(MPI_COMM_WORLD, &mpiProcesses);
	/*voy a armar comunicadores*/
	createCommunicator( &commNodes, &groupNodes, &groupWorld, &processRank, mpiProcesses, idNodo );
	MPI_Comm_rank( commNodes, &idNodoInterno);
	
	switch( idNodo ){
		case MASTER_ID:
			//benchmark code
			//master(mpiProcesses, commNodes,filenameJson, benckmarkCsv);
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
				printer();
			}else {
				printf("Master node has sent BAD_JSON by broadcast\n");
			}
			break;
		default :
			MPI_Bcast_JSON( &jsonResult, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
			if ( jsonResult == GOOD_JSON ) {
				genericNode(idNodo, idNodoInterno, commNodes,mpiProcesses);
			}
			break;
	}
	//benchmark code
	/*if(idNodo == MASTER_ID){
		clock_gettime(USED_CLOCK, &endTime);
		end = endTime.tv_sec * NANOS + endTime.tv_nsec;
		runningTime = (uint64_t)(end - start);
		write(benckmarkCsv,"\t",1);
		putUnsigned(benckmarkCsv, runningTime);
		write(benckmarkCsv,"\n",1);
		close(benckmarkCsv);
	}*/

	/* FIN de zona de MPI */
	if(processRank != NULL)free(processRank);
	MPI_Finalize();
	return 0;
}

//benchmark code
//void master(const int mpiProcesses, const MPI_Comm commNodes ,const char *filenameJson , int benckmarkCsv){
void master(const int mpiProcesses, const MPI_Comm commNodes ,const char *filenameJson ){
	//benchmark code
	/*struct timespec startLoadTime, endLoadTime;
	long long start = 0LL, end = 0LL;
	long long loadTime = 0LL;
	struct timespec startSIMTime, endSIMTime;
	long long simTime = 0LL;*/

	int jsonResult;

	//benchmark code
	/*clock_gettime(USED_CLOCK, &startLoadTime);
	start = startLoadTime.tv_sec * NANOS + startLoadTime.tv_nsec;*/

	ValidationResults* vr = validateJsonInput(filenameJson);
	if ( vr->isValid == VALIDATION_PASS ) {		
		if ( vr->nodesAmount + MASTER_RAFFLER_PRINTER == mpiProcesses ) {
			

			//broadcast TAG JSON BUENO
			jsonResult = GOOD_JSON;
			MPI_Bcast_JSON( &jsonResult, 1, MPI_INT, MASTER_ID, MPI_COMM_WORLD);
			//envio estructuras
			sendStructToNodes(filenameJson, commNodes);
			//enviar lo combisIds al raffler
			MPI_Send( &vr->seedAndCombisId[1] ,   vr->seedAndCombisId[0] , MPI_INT , RAFFLER_ID , SEED_AND_COMBI_LIST , MPI_COMM_WORLD);
			//envio counts de los elementos.
			MPI_Send( vr->qCouNfComb , 5 , MPI_INT , PRINTER_ID , INIT_NODES , MPI_COMM_WORLD);

			//benchmark code
			/*clock_gettime(USED_CLOCK, &endLoadTime);
			end = endLoadTime.tv_sec * NANOS + endLoadTime.tv_nsec;
			loadTime = (uint64_t)(end - start);
			write(benckmarkCsv,"\t",1);
			putUnsigned(benckmarkCsv, loadTime);*/
			//----
			/*clock_gettime(USED_CLOCK, &startSIMTime);
			start = startSIMTime.tv_sec * NANOS + startSIMTime.tv_nsec;*/

			scheduler( vr->watchdog, commNodes , vr->targets , mpiProcesses, vr->targetCounter);

			//benchmark code
			/*clock_gettime(USED_CLOCK, &endSIMTime);
			end = endSIMTime.tv_sec * NANOS + endSIMTime.tv_nsec;
			simTime = (uint64_t)(end - start);
			write(benckmarkCsv,"\t",1);
			putUnsigned(benckmarkCsv, simTime);*/

			free(vr->targets);
			free(vr->seedAndCombisId);
			free(vr->qCouNfComb);
			free(vr);
			return;
		}
		else {
			const char* label = "Error en la cantidad de nodos contra procesos\n";
			char* botqueueOutputFile = getenv("BOTQUEUE_OUTPUT_FILE");
			if(botqueueOutputFile == NULL ){
				botqueueOutputFile = "/tmp/defaultOutputJson.json";
				printf("Process Printer: can not find BOTQUEUE_OUTPUT_FILE, using default path: /tmp/defaultOutputJson.json \n");
			}
			int fileDescriptor = open (botqueueOutputFile , O_WRONLY|O_CREAT|O_TRUNC,00660);
			write(fileDescriptor,"{\n\"Error\" : \"",13);
			write(fileDescriptor, label, strlen(label) );
			write(fileDescriptor,"\"\n}",3);
			close(fileDescriptor);
		}
	}
	free(vr); 
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

//benchmark code
/*void putUnsigned(int fileDescriptor, const long long nro){
	char* strNro = NULL;
	int len = snprintf(NULL, 0, "%lld", nro);
	strNro = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(strNro, (len + 1), "%lld", nro);
	write(fileDescriptor, strNro, len );
	free(strNro);
}*/