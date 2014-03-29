#include "ourMPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "printer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void printer(){
	int fileDescriptor;
	double totalTime = 0.0;
	int flag = FALSE;
	MPI_Status result;
	
	
	//recive del master la cantidad de nodos
	int* qCouNfComb = (int*)malloc(sizeof(int)*5);
	MPI_Recv(qCouNfComb, 5, MPI_INT, MASTER_ID, INIT_NODES , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
	char* botqueueOutputFile = getenv("BOTQUEUE_OUTPUT_FILE");
	if(botqueueOutputFile == NULL ){
		botqueueOutputFile = "/tmp/defaultOutputJson.json";
		printf("Process Printer: can not find BOTQUEUE_OUTPUT_FILE, using default path: /tmp/defaultOutputJson.json \n");
	}
	
	fileDescriptor = open (botqueueOutputFile , O_WRONLY|O_CREAT|O_TRUNC,00660);
	
	//open json file
	openBrace(fileDescriptor);
	enter(fileDescriptor);
	putLabel(fileDescriptor, "timeLines"); 
		openBracket(fileDescriptor);
		enter(fileDescriptor);
		do{//te llama el scheduler y te dice que se va ha consumir un delta t, por lo que todos vienen
			MPI_Recv6(&totalTime, 1, MPI_DOUBLE, MASTER_ID, MPI_ANY_TAG , MPI_COMM_WORLD, &result);
			MockResult(&result);
			if(result.MPI_TAG == PRINT_SIGNAL ){
				//printf("recivi el fucking PRINT_SIGNAL\n");
				if(flag == TRUE){
					separeElement(fileDescriptor);
					enter(fileDescriptor);
				} 
				doDeltaT(fileDescriptor, totalTime, qCouNfComb[0], qCouNfComb[1], qCouNfComb[2], qCouNfComb[3], qCouNfComb[4]);
				flag = TRUE;

			}
			MockLoop(&result);//si es test termina el bucle
		}while(result.MPI_TAG != LIVE_LOCK);	

	closeBracket(fileDescriptor); separeElement(fileDescriptor);enter(fileDescriptor);
	putLabel(fileDescriptor, "summaryReport"); 
		doSummaryReport(fileDescriptor, totalTime, qCouNfComb[0], qCouNfComb[1]);
	
	//close json file
	closeBrace(fileDescriptor);
	
	close(fileDescriptor);
	free(qCouNfComb);
}

void doSummaryReport(int fileDescriptor, const double totalTime, const int queues, const int counters){
	PrinterFinalCounter crStruct;
	PrinterFinalQueue qeStruct;
	double totalCost = 0.0;
	//open summary
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "totalTime"); putDouble(fileDescriptor, totalTime); separeElement(fileDescriptor);

		putLabel(fileDescriptor, "counters"); openBracket( fileDescriptor);enter(fileDescriptor);
			//recibo todos los envios de colas
			for(int i = 0; i < counters; i++){
				MPI_Recv3(&crStruct, sizeof(PrinterFinalCounter), MPI_BYTE, MPI_ANY_SOURCE, COUNTER_FINAL_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				doFinalCounter( fileDescriptor, crStruct.idNode, crStruct.totalProductivity);
				if (i+1 < counters ){separeElement(fileDescriptor); enter(fileDescriptor);}
			}
		closeBracket(fileDescriptor); separeElement(fileDescriptor);enter(fileDescriptor);

		putLabel(fileDescriptor, "queues"); openBracket( fileDescriptor);
			//recibo todos los envios de colas
			for(int i = 0; i < queues; i++){
				MPI_Recv3(&qeStruct, sizeof(PrinterFinalQueue), MPI_BYTE, MPI_ANY_SOURCE, QUEUE_FINAL_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				//qeStruct.VariableCost es el constante del costo variable * el numero de recursos que salieron de la cola.
				totalCost +=  qeStruct.fixCost + qeStruct.VariableCost;
				doFinalQueue( fileDescriptor, qeStruct.idNode, qeStruct.fixCost, qeStruct.VariableCost );
				if (i+1 < queues ){separeElement(fileDescriptor);}
			}
		closeBracket(fileDescriptor); separeElement(fileDescriptor);enter(fileDescriptor);
					
		putLabel(fileDescriptor, "totalCost"); putDouble(fileDescriptor, totalCost);

	//close summary
	closeBrace(fileDescriptor);

}


void doFinalCounter(int fileDescriptor, const int idNode, const int totalProductivity){
	//open one Counter
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "idNode");   putInteger(fileDescriptor, idNode); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "totalProductivity");   putInteger(fileDescriptor, totalProductivity); 
	//close one Counter
	closeBrace(fileDescriptor);
}


void doFinalQueue(int fileDescriptor, const int idNode, const double fixCost, const double variableCost){
	//open one queue
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "idNode");   putInteger(fileDescriptor, idNode); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "fixCost");   putDouble(fileDescriptor, fixCost); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "variableCost");   putDouble(fileDescriptor, variableCost);
	//close one queue
	closeBrace(fileDescriptor);
}

void doDeltaT(int fileDescriptor, const double deltaT, const int queues, const int counters, const int normals, const int functions, const int combis){
	PrinterActivity cbStruct;
	PrinterActivity nlStruct;
	PrinterCounter crStruct;
	PrinterQueue qeStruct;
	PrinterFunction fnStruct;
	int oldSource = 0;
	MPI_Status result;
	//open one delta T
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "deltaT");   putDouble(fileDescriptor, deltaT); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "nodesStatus"); openBrace(fileDescriptor);
			putLabel(fileDescriptor, "queues"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
				for(int i = 0; i < queues; i++){
					//obtengo estructura
					//printf("recividela cola\n");
					MPI_Recv3(&qeStruct, sizeof(PrinterQueue), MPI_BYTE, MPI_ANY_SOURCE, QUEUE_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					doQueue( fileDescriptor, qeStruct.idNode, qeStruct.amount, qeStruct.counterInput, qeStruct.counterOutput, 
							qeStruct.average, qeStruct.maximun,  qeStruct.minimun,  qeStruct.timesNotEmpty,  qeStruct.percentTimesNotEmpty);
					if (i+1 < queues ){separeElement(fileDescriptor);}
				}
			closeBracket(fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "counters"); openBracket( fileDescriptor);
				CycleValidator * counterCycles = (CycleValidator*) malloc(sizeof (CycleValidator) * counters);
				//recibo todos los envios de colas
				for(int i = 0; i < counters; i++){
					//obtengo estructura
					//printf("recividela counters\n");
					MPI_Recv3(&crStruct, sizeof(PrinterCounter), MPI_BYTE, MPI_ANY_SOURCE, COUNTER_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					counterCycles[i].idNode =  crStruct.idNode;
					counterCycles[i].cycle =  crStruct.totalProductivity;
					doCounter( fileDescriptor, crStruct.idNode, crStruct.totalProductivity, crStruct.deltaTProductivity, crStruct.productivityPerTime );
					if (i+1 < counters ){separeElement(fileDescriptor);}
				}
				MPI_Send(counterCycles, counters*sizeof(CycleValidator), MPI_BYTE, MASTER_ID, COUNTER_CYCLES, MPI_COMM_WORLD);
				free(counterCycles);
			closeBracket( fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "normals"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
				for(int i = 0; i < normals; i++){
					//obtengo estructura
					MPI_Recv4(&nlStruct, sizeof(PrinterActivity), MPI_BYTE, MPI_ANY_SOURCE, NORMAL_REPORT , MPI_COMM_WORLD, &result);
					
					oldSource = result.MPI_SOURCE;
					double* worktask = NULL;
					double* worktaskDelay = NULL;
					if(nlStruct.activityInside > 0 ){
						worktask = (double*) malloc( nlStruct.activityInside* 2 * sizeof(double) );
						//los delay internos
						MPI_Recv5(worktask, nlStruct.activityInside* 2, MPI_DOUBLE, oldSource, NORMAL_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						worktaskDelay = &worktask[nlStruct.activityInside];
					}
					doActivity(fileDescriptor, nlStruct.idNode, nlStruct.activityInside, worktask, 	worktaskDelay, nlStruct.counterInput, nlStruct.amountDelay,  nlStruct.maximunDrawn, nlStruct.minimunDrawn );
					if(nlStruct.activityInside > 0 ){
						free(worktask);
						worktask = NULL;
					}
					
					
					if (i+1 < normals ){separeElement(fileDescriptor);}
					
				}
			closeBracket( fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "functions"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
				for(int i = 0; i < functions; i++){
					//obtengo estructura
					MPI_Recv3(&fnStruct, sizeof(PrinterFunction), MPI_BYTE, MPI_ANY_SOURCE, FUNCTION_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					doFunction( fileDescriptor, fnStruct.idNode, fnStruct.amount);
					if (i+1 < functions ){separeElement(fileDescriptor);}
				}
			closeBracket( fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "combis"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
				for(int i = 0; i < combis; i++){
					//obtengo estructura
					//printf("HHHHHHHHHHHHHHHHHHH %d\n",combis );
					MPI_Recv4(&cbStruct, sizeof(PrinterActivity), MPI_BYTE, MPI_ANY_SOURCE, COMBI_REPORT , MPI_COMM_WORLD, &result);
					oldSource = result.MPI_SOURCE;
					double* worktask = NULL;
					double * worktaskDelay = NULL;
					if(cbStruct.activityInside > 0){
						worktask = (double*) malloc( cbStruct.activityInside* 2 * sizeof(double) );
						//los delay internos
						
						MPI_Recv5(worktask, cbStruct.activityInside* 2, MPI_DOUBLE, oldSource, COMBI_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						worktaskDelay = &worktask[nlStruct.activityInside];
					}
					doActivity(fileDescriptor, cbStruct.idNode, cbStruct.activityInside, worktask, worktaskDelay, cbStruct.counterInput, cbStruct.amountDelay, cbStruct.maximunDrawn, cbStruct.minimunDrawn );
					if(cbStruct.activityInside > 0){
						free(worktask);
						worktask = NULL;
					}
					if (i+1 < combis ){separeElement(fileDescriptor);}
						
				}
			closeBracket( fileDescriptor);
		//close nodestatus
		closeBrace(fileDescriptor);
	//close one delta T
	closeBrace(fileDescriptor);
}

void doActivity(int fileDescriptor, const int idNode, const int activityInside, const double* contdownWorktask, 
						const double* delayWorktask, const int counterInput, const double amountDelay, 
						const double maximunDrawn, const double minimunDrawn ){
	//open one Activity
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "idNode");   putInteger(fileDescriptor, idNode); separeElement(fileDescriptor);

		putLabel(fileDescriptor, "contdownWorktask"); openBracket( fileDescriptor);
		if (activityInside > 0){
			putDouble(fileDescriptor, contdownWorktask[0]);
		}
		for(int i =1 ; i < activityInside; i++){
			separeElement(fileDescriptor);
			putDouble(fileDescriptor, contdownWorktask[i]);
		} closeBracket( fileDescriptor); separeElement(fileDescriptor);

		putLabel(fileDescriptor, "delayWorktask"); openBracket( fileDescriptor);
		if (activityInside > 0){
			putDouble(fileDescriptor, delayWorktask[0]);
		}
		for(int i =1 ; i < activityInside; i++){
			separeElement(fileDescriptor);
			putDouble(fileDescriptor, delayWorktask[i]);
		} closeBracket( fileDescriptor); separeElement(fileDescriptor);

		putLabel(fileDescriptor, "counterInput");   putInteger(fileDescriptor, counterInput); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "amountDelay");   putDouble(fileDescriptor, amountDelay); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "maximunDrawn");
		putDouble(fileDescriptor, maximunDrawn);
		separeElement(fileDescriptor);
		putLabel(fileDescriptor, "minimunDrawn");
		putDouble(fileDescriptor, minimunDrawn);
	//close one Activity
	closeBrace(fileDescriptor);
}

void doFunction(int fileDescriptor, const int idNode, const int amount){
	//open one Function
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "idNode");   putInteger(fileDescriptor, idNode); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "amount");   putInteger(fileDescriptor, amount); 
	//close one Function
	closeBrace(fileDescriptor);
}

void doCounter(int fileDescriptor, const int idNode, const int totalProductivity, const int deltaTProductivity, const double productivityPerTime ){
	//open one Counter
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "idNode");   putInteger(fileDescriptor, idNode); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "totalProductivity");   putInteger(fileDescriptor, totalProductivity); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "deltaTProductivity");   putInteger(fileDescriptor, deltaTProductivity); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "productivityPerTime");   putDouble(fileDescriptor, productivityPerTime);
	//close one Counter
	closeBrace(fileDescriptor);
}

void doQueue(int fileDescriptor, const int idNode, const int amount, const int counterInput, const int counterOutput, 
			const double average, const int maximun,  const int minimun,  const double timesNotEmpty,  const double percentTimesNotEmpty){
	//open one queue
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "idNode");   putInteger(fileDescriptor, idNode); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "amount");   putInteger(fileDescriptor, amount); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "counterInput");   putInteger(fileDescriptor, counterInput); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "counterOutput");   putInteger(fileDescriptor, counterOutput); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "average");   putDouble(fileDescriptor, average); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "maximun");   putInteger(fileDescriptor, maximun); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "minimun");   putInteger(fileDescriptor, minimun); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "timesNotEmpty");   putDouble(fileDescriptor, timesNotEmpty); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "percentTimesNotEmpty");   putDouble(fileDescriptor, percentTimesNotEmpty);
	//close one queue
	closeBrace(fileDescriptor);
}

void openBrace(int fileDescriptor){
	write(fileDescriptor,"{",1);
}

void closeBrace(int fileDescriptor){
	write(fileDescriptor,"}",1);
}

void openBracket(int fileDescriptor){
	write(fileDescriptor,"[",1);
}

void closeBracket(int fileDescriptor){
	write(fileDescriptor,"]",1);
}

void separeElement(int fileDescriptor){
	write(fileDescriptor,", ",2);
}

void enter(int fileDescriptor){
	write(fileDescriptor,"\n",1);
}

void putLabel(int fileDescriptor, const char* label){
	write(fileDescriptor,"\"",1);
	write(fileDescriptor, label, strlen(label) );
	write(fileDescriptor,"\" : ",4);
}

void putInteger(int fileDescriptor, const int nro){
	char* strNro = NULL;
	int len = snprintf(NULL, 0, "%d", nro);
	strNro = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(strNro, (len + 1), "%d", nro);
	write(fileDescriptor, strNro, len );
	free(strNro);
}

void putUnsigned(int fileDescriptor, const unsigned nro){
	char* strNro = NULL;
	int len = snprintf(NULL, 0, "%u", nro);
	strNro = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(strNro, (len + 1), "%u", nro);
	write(fileDescriptor, strNro, len );
	free(strNro);
}

void putDouble(int fileDescriptor, const double nro){
	char* strNro = NULL;
	if( isnan(nro) ){
		write(fileDescriptor, "null", 4 );
	} else {
		int len = snprintf(NULL, 0, "%lf", nro);
		strNro = (char*) malloc( (len + 1) * sizeof(char) );
		snprintf(strNro, (len + 1), "%lf", nro);
		write(fileDescriptor, strNro, len );
		free(strNro);
	}

	
}
