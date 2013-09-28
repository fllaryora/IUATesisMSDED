#include "ourMPI.h"
#include <stdio.h>
#include "printer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//TODO terminar
void printer(){
	int fileDescriptor;

	printf("Hola desde el printer\n");
	
	fileDescriptor = open ("/tmp/pijeriasDeJson.txt",O_WRONLY|O_CREAT,00660);

	//open json file
	openBrace(fileDescriptor);
	putLabel(fileDescriptor, "timeLine"); openBrace(fileDescriptor);
		openBracket(fileDescriptor);
			//te llama el scheduler y te dice que se va ha consumir un delta t, por lo que todos vienen
			//y despues vienen todos
			//separeElement(fileDescriptor); ente llamados
			doDeltaT(fileDescriptor, deltaT);
		closeBrace(fileDescriptor);
	closeBracket(fileDescriptor)
	putLabel(fileDescriptor, "summaryReport"); openBrace(fileDescriptor);
		doSummaryReport(fileDescriptor);
	closeBrace(fileDescriptor);

	//close json file
	closeBrace(fileDescriptor);

	close(fileDescriptor);
}

//TODO terminar
void doDeltaT(int fileDescriptor, const double deltaT){
	//open one delta T
	openBrace(fileDescriptor);
		putLabel(fileDescriptor, "deltaT");   putDouble(fileDescriptor, deltaT); separeElement(fileDescriptor);
		putLabel(fileDescriptor, "nodesStatus"); openBrace(fileDescriptor);
			putLabel(fileDescriptor, "queues"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
			closeBracket(int fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "counters"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
			closeBracket(int fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "normals"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
			closeBracket(int fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "functions"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
			closeBracket(int fileDescriptor); separeElement(fileDescriptor);
			putLabel(fileDescriptor, "combis"); openBracket( fileDescriptor);
				//recibo todos los envios de colas
			closeBracket(int fileDescriptor);
		//close nodestatus
		closeBrace(fileDescriptor);
	//close one delta T
	closeBrace(fileDescriptor);
}

//TODO terminar
void doSummaryReport(int fileDescriptor){
	double totalTime = 0.0;
	//llega la estructura de ?? (totalTime, totalCost, cantidad de counters, cantidad de queues)
	putLabel(fileDescriptor, "totalTime"); putDouble(fileDescriptor, totalTime); separeElement(fileDescriptor);
	putLabel(fileDescriptor, "totalCost"); putDouble(fileDescriptor, totalCost); separeElement(fileDescriptor);
	//llegan los counters de ??
	putLabel(fileDescriptor, "counters");  separeElement(fileDescriptor);
	//llegan las queue de ??
	putLabel(fileDescriptor, "queues"); 
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
		} closeBracket(int fileDescriptor); separeElement(fileDescriptor);

		putLabel(fileDescriptor, "delayWorktask"); openBracket( fileDescriptor);
		if (activityInside > 0){
			putDouble(fileDescriptor, delayWorktask[0]);
		}
		for(int i =1 ; i < activityInside; i++){
			separeElement(fileDescriptor);
			putDouble(fileDescriptor, delayWorktask[i]);
		} closeBracket(int fileDescriptor); separeElement(fileDescriptor);

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
