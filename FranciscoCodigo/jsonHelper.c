#include "jsonHelper.h"
#include <stdio.h>
#include <stdlib.h>

int getNodesAmount( void ){
	return 1;
}

int validateJsonInput( void ){
	return JSON_APPROVED;
}

void putNodesInMem( void ){
	return;
}

void* getNodeById( int id){
	return NULL;
}

int* getCombiIds( void ){
	int count = 11;
	int* ids = (int*)malloc(sizeof(int) * count);
	ids[0] = count;
	ids[1] = -1;
	for(int i = 2 ; i < count; i++){ ids[i]=i; }
	return ids;
}
