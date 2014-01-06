#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "jsonHelper.h"

int main(){
	int jsonResult;
	ValidationResults* vr = validateJsonInput("archivos/modelo.json");
	if ( vr->isValid == VALIDATION_PASS ) {		
		if ( vr->nodesAmount + MASTER_RAFFLER_PRINTER == 29 ) {
			free(vr->targets);
			free(vr->seedAndCombisId);
			free(vr->targets);
			free(vr);
			return 0;
		}
		else {
			printf("Error en la cantidad de nodos contra procesos\n");
		}
	}
	free(vr); 
	return 0;
}