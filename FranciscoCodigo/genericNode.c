#include "ourMPI.h"
#include "genericNode.h"
#include "jsonHelper.h"
#include <stdio.h>
void genericNode(int myIdNodo){
	getNodeById( myIdNodo );
	printf("Hello from slave\n");
}
