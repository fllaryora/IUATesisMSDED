#include "ourMPI.h"
#include "main.h"
#include "genericNode.h"
#include "jsonHelper.h"
#include <stdio.h>
#include <stdlib.h>

void genericNode(int myIdNodo){
	
	getNodeById( myIdNodo );
	printf("Hello from slave\n");
	
}


