#include "ourMPI.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"


void scheduler(unsigned long watchdog, const MPI_Comm commNodes , CycleValidator* targets, const int mpiProcesses, const int counterNodes){
	int msg = 0;
	int watchdog2 = 0;
	int* nodesStatus = (int*) malloc (sizeof(int)  * (mpiProcesses -RAFFLER_PRINTER));
	CycleValidator* currentStatusOfTargets = (CycleValidator*) malloc (sizeof(CycleValidator)  * counterNodes);
	int  isAllFinalized = 0;
	
	double totalTime = 0.0;
	
	do{//comienza las faces de nuevo

		//New rafle
		MPI_Send( NULL , 0 , MPI_INT , RAFFLER_ID , NEW_RAFFLE , MPI_COMM_WORLD);

		msg = GENERATION_PHASE;
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
		MPI_Barrier( commNodes );

		msg = ADVANCE_PAHSE;
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
		MPI_Barrier( commNodes );
		
		//heuristica anti bucles infinitos en el modelo
		watchdog2 = (int) (1.4 *  (mpiProcesses -MASTER_RAFFLER_PRINTER));

		do{
			
			msg = GENERATION_PHASE_PRIMA;
			MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
			MPI_Barrier( commNodes );

			msg = ADVANCE_PAHSE_PRIMA;
			MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
			
			//calculo que todos los nodos hayan finalizado
			isAllFinalized = TRUE;
			msg = TRUE; //no importa
			MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, 1 , MPI_INT,  MASTER_ID, commNodes);
			for(int i = 1 ; i< (mpiProcesses - RAFFLER_PRINTER);i++ ){
				//basta que uno no cumpla
				if(!(isAllFinalized &=  nodesStatus[i])) break;
			}

		// !isAllFinalized  == si algun nodo no finalizo -> seguir
		}while(((watchdog2--) > 0) && !isAllFinalized );
		
		msg = CONSUME_DT;
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
		MPI_Barrier( commNodes );
		watchdog--;
		
		//New printer
		MPI_Send( &totalTime , 1 , MPI_DOUBLE , PRINTER_ID , PRINT_SIGNAL , MPI_COMM_WORLD);
		//todos los nodos deben reportarse al printer
		msg = PING_REPORT;
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
		
		MPI_Recv( currentStatusOfTargets, counterNodes*sizeof(CycleValidator), MPI_BYTE, PRINTER_ID, COUNTER_CYCLES, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		isAllFinalized = 0;
		
		for(int i = 0 ; i< counterNodes;i++ ){
			for(int j = 0 ; j< counterNodes;j++ ){
				if(currentStatusOfTargets[i].idNode == targets[j].idNode){
					if( currentStatusOfTargets[i].cycle >= targets[j].cycle )
						isAllFinalized++;
					else{
						j= i = counterNodes;
						//basta que uno no cumpla para salir
					}
				}	
			}
		}
		
	printf("-- FIN DELTA T --\n");
	totalTime += TIME_ONE_STEP;
	} while( watchdog > 0 && (isAllFinalized!=counterNodes));
	//envio livelock al resto de los nodos
	printf("SALIOOO del Watchdog\n");
	msg = LIVE_LOCK;
	MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID, commNodes);
	MPI_Send( NULL , 0 , MPI_INT , RAFFLER_ID , LIVE_LOCK , MPI_COMM_WORLD);
	MPI_Send(&totalTime, 1, MPI_DOUBLE, PRINTER_ID , LIVE_LOCK , MPI_COMM_WORLD);

	free(nodesStatus);
	free(currentStatusOfTargets);

}
