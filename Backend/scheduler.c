#include "ourMPI.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

#define WATCHDOG_DOESNT_BITE_ME(X) (X >= 0)

void scheduler(unsigned long watchdog, const MPI_Comm commNodes , const int * const targets , const int mpiProcesses, const int counterNodes){
	int msg = 0;
	int watchdog2 = 0;
	int* nodesStatus = (int*) malloc (sizeof(int)  * (mpiProcesses -RAFFLER_PRINTER));
	int* targetStatus = (int*) malloc (sizeof(int)  * counterNodes);
	int  isAllFinalized = 0;
	printf("Hello from master\n");
	double totalTime = 0.0;
	//TODO sacal el siguente comentario
	//definido en la fotografia que envie por watzap y no vio chen
	
	do{//comienza las faces de nuevo
	
		//New rafle
		MPI_Send( NULL , 0 , MPI_INT , RAFFLER_ID , NEW_RAFFLE , MPI_COMM_WORLD);
		msg = ADVANCE_PAHSE;
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
		MPI_Barrier( commNodes );

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
			MPI_Gather(&msg, 1, MPI_INT,  nodesStatus, (mpiProcesses - RAFFLER_PRINTER) , MPI_INT,  MASTER_ID, commNodes);
			for(int i = 1 ; i< (mpiProcesses - RAFFLER_PRINTER);i++ ){
				//basta que uno no cumpla
				if(!(isAllFinalized &=  nodesStatus[i])) break;
			}

		}while(((watchdog--) > 0) && !isAllFinalized );
		msg = GENERATION_PHASE;
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
		MPI_Barrier( commNodes );

		msg = CONSUME_DT;
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
		MPI_Barrier( commNodes );

		//New printer
		MPI_Send( &totalTime , 1 , MPI_DOUBLE , PRINTER_ID , PRINT_SIGNAL , MPI_COMM_WORLD);
		//todos los nodos deben reportarse al printer
		msg = PING_REPORT;
		MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);

		MPI_Recv( targetStatus, counterNodes, MPI_INT, PRINTER_ID, COUNTER_CYCLES, , MPI_COMM_WORLD);
		isAllFinalized = 0;
		for(int i = 0 ; i< counterNodes;i++ ){
			//basta que uno no cumpla
			if(!(targetStatus[i] >= targets[i]))  break;
			isAllFinalized++;
		}

	} while( ((watchdog--) > 0) && (isAllFinalized < counterNodes) ;
	//envio livelock al resto de los nodos
	msg = LIVE_LOCK;
	MPI_Bcast( &msg ,1,MPI_INT, MASTER_ID,commNodes);
	MPI_Send( NULL , 0 , MPI_INT , RAFFLER_ID , LIVE_LOCK , MPI_COMM_WORLD);
	MPI_Send(&totalTime, 1, MPI_DOUBLE, MPI_INT , PRINTER_ID , LIVE_LOCK , MPI_COMM_WORLD);

	free(nodesStatus);
	free(targetStatus);

}


