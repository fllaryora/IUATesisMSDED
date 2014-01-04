#include <stdio.h>
#include <stdlib.h>
#include "jsonHelperN.h"

int main(){
	ValidationResults* vr = validateJsonInput("archivos/modelo.json");
	if ( vr->isValid == VALIDATION_PASS ) {		
		if ( vr->nodesAmount + 3 == 29 ) {
			free(vr->targets);
			free(vr->seedAndCombisId);
			free(vr->qCouNfComb);
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
