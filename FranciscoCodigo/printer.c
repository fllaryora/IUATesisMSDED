#include "ourMPI.h"
#include <stdio.h>
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
	int* qCouNfComb = (int*) malloc( 5 * sizeof(int) );
	//recive del master la cantidad de nodos
	GetEachNodesAmount(qCouNfComb);
	fileDescriptor = open ("/tmp/pijeriasDeJson.txt",O_WRONLY|O_CREAT,00660);
	
	//open json file
	openBrace(fileDescriptor);
	putLabel(fileDescriptor, "timeLine"); 
		openBracket(fileDescriptor);

		do{//te llama el scheduler y te dice que se va ha consumir un delta t, por lo que todos vienen
			WaitForPrinterSignal(&totalTime, &result);
			if(result.MPI_TAG == PRINT_SIGNAL ){
				if(flag == TRUE){
					separeElement(fileDescriptor);
				} 
				doDeltaT(fileDescriptor, totalTime, qCouNfComb[0], qCouNfComb[1], qCouNfComb[2], qCouNfComb[3], qCouNfComb[4]);
				flag = TRUE;
			}
			MockLoop(&result);//si es test termina el bucle
		}while(result.MPI_TAG != LIVE_LOCK);	

	closeBracket(fileDescriptor); separeElement(fileDescriptor);
	putLabel(fileDescriptor, "summaryReport"); 
		doSummaryReport(fileDescriptor, totalTime, qCouNfComb[0], qCouNfComb[1]);
	
	//close json file
	closeBrace(fileDescriptor);
	free(qCouNfComb);
	close(fileDescriptor);
}

void doSummaryReport(int fileDescriptor, const double totalTime, const int queues, const int counters){
	PrinterFinalCounter crStruct;
	PrinterFinalQueue qeStruct;
	double totalCost = 0.0;
	//open summary
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "totalTime"); putDouble(fileDescriptor, totalTime); separeElement(fileDescriptor);

		putLabel(fileDescriptor, "counters"); openBracket( fileDescriptor);
			//recibo todos los envios de colas
			for(int i = 0; i < counters; i++){
				GetFinalCounterStruct(&crStruct);
				doFinalCounter( fileDescriptor, crStruct.idNode, crStruct.totalProductivity);
				if (i+1 < counters ){separeElement(fileDescriptor);}
			}
		closeBracket(fileDescriptor); separeElement(fileDescriptor);

		putLabel(fileDescriptor, "queues"); openBracket( fileDescriptor);
			//recibo todos los envios de colas
			for(int i = 0; i < queues; i++){
				GetFinalQueueStruct(&qeStruct);
				//qeStruct.VariableCost es el constante del costo variable * el numero de recursos que salieron de la cola.
				totalCost +=  qeStruct.fixCost + qeStruct.VariableCost;
				doFinalQueue( fileDescriptor, qeStruct.idNode, qeStruct.fixCost, qeStruct.VariableCost );
				if (i+1 < queues ){separeElement(fileDescriptor);}
			}
		closeBracket(fileDescriptor); separeElement(fileDescriptor);
					
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


void doFinalQueue(int fileDescriptor, const int idNode, const double fixCost, const double VariableCost){
	//open one queue
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "idNode");   putInteger(fileDescriptor, idNode); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "fixCost");   putDouble(fileDescriptor, fixCost); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "VariableCost");   putDouble(fileDescriptor, VariableCost);
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
					GetQueueStruct(&qeStruct);
					doQueue( fileDescriptor, qeStruct.idNode, qeStruct.amount, qeStruct.counterInput, qeStruct.counterOutput, 
							qeStruct.average, qeStruct.maximun,  qeStruct.minimun,  qeStruct.timesNotEmpty,  qeStruct.percentTimesNotEmpty);
					if (i+1 < queues ){separeElement(fileDescriptor);}
				}
			closeBracket(fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "counters"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
				for(int i = 0; i < counters; i++){
					GetCounterStruct(&crStruct);
					doCounter( fileDescriptor, crStruct.idNode, crStruct.totalProductivity, crStruct.deltaTProductivity, crStruct.productivityPerTime );
					if (i+1 < counters ){separeElement(fileDescriptor);}
				}
			closeBracket( fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "normals"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
				for(int i = 0; i < normals; i++){
					GetNormalStruct(&nlStruct, &result);
					oldSource = result.MPI_SOURCE;
					double* worktask = (double*) malloc( nlStruct.activityInside* 2 * sizeof(double) );
					GetNormalDalaysStruct(worktask, nlStruct.activityInside* 2, oldSource);
					doActivity(fileDescriptor, nlStruct.idNode, nlStruct.activityInside, worktask, 
						&worktask[nlStruct.activityInside], nlStruct.counterInput, nlStruct.delayAverage, 
						nlStruct.maximunDrawn, nlStruct.minimunDrawn );
					if (i+1 < normals ){separeElement(fileDescriptor);}
					free(worktask);
				}
			closeBracket( fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "functions"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
				for(int i = 0; i < functions; i++){
					GetFunctionStruct(&fnStruct);
					doFunction( fileDescriptor, fnStruct.idNode, fnStruct.amount);
					if (i+1 < functions ){separeElement(fileDescriptor);}
				}
			closeBracket( fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "combis"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
				for(int i = 0; i < combis; i++){
					GetCombiStruct(&cbStruct, &result);
					oldSource = result.MPI_SOURCE;
					double* worktask = (double*) malloc( cbStruct.activityInside* 2 * sizeof(double) );
					GetNormalDalaysStruct(worktask, cbStruct.activityInside* 2, oldSource);
					doActivity(fileDescriptor, cbStruct.idNode, cbStruct.activityInside, worktask, 
						&worktask[cbStruct.activityInside], cbStruct.counterInput, cbStruct.delayAverage, 
						cbStruct.maximunDrawn, cbStruct.minimunDrawn );
					if (i+1 < combis ){separeElement(fileDescriptor);}
					free(worktask);
				}
			closeBracket( fileDescriptor);
		//close nodestatus
		closeBrace(fileDescriptor);
	//close one delta T
	closeBrace(fileDescriptor);
}



//#define doNormal(A , B , C, D, E, F, G, H, I )	doActivity(A , B , C, D, E, F, G, H, I );
//#define doCombi(A , B , C, D, E, F, G, H, I )	doActivity(A , B , C, D, E, F, G, H, I );
void doActivity(int fileDescriptor, const int idNode, const int activityInside, const double* contdownWorktask, 
						const double* delayWorktask, const int counterInput, const double delayAverage, 
						const int maximunDrawn, const int minimunDrawn ){
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
		putLabel(fileDescriptor, "delayAverage");   putDouble(fileDescriptor, delayAverage); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "maximunDrawn");   putInteger(fileDescriptor, maximunDrawn); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "minimunDrawn");   putInteger(fileDescriptor, minimunDrawn); 
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
		putLabel(fileDescriptor, "minimun");   putInteger(fileDescriptor, timesNotEmpty); separeElement(fileDescriptor);
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

void putDouble(int fileDescriptor, const double nro){
	char* strNro = NULL;
	int len = snprintf(NULL, 0, "%lf", nro);
	strNro = (char*) malloc( (len + 1) * sizeof(char) );
	snprintf(strNro, (len + 1), "%lf", nro);
	write(fileDescriptor, strNro, len );
	free(strNro);
}

void putArrayDouble(int fileDescriptor, const double* array, int size){
	openBracket(fileDescriptor);
	if(size > 0){
		putDouble(fileDescriptor, array[0]);
		for(int i = 1; i < size;i++){
			separeElement(fileDescriptor);
			putDouble( fileDescriptor, array[i]);
		}
	}
	closeBracket(fileDescriptor);
}

void putArrayInteger(int fileDescriptor, const int* array, int size){
	openBracket(fileDescriptor);
	if(size > 0){
		putInteger(fileDescriptor, array[0]);
		for(int i = 1; i < size;i++){
			separeElement(fileDescriptor);
			putInteger( fileDescriptor, array[i]);
		}
	}
	closeBracket(fileDescriptor);
}
