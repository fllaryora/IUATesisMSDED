#include "validador.h"
/* #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wjelement.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>*/

#include <string.h>

#define STREQ(A, B) (A && B ? strcmp(A, B) == 0 : 0)


int main(int argc, char **argv){
	int rta;
	const char *filenameJson   = "archivos/modelo.json";
	const char *filenameSchema = "archivos/schema.json";

	if (validateSchema(filenameJson , filenameSchema) != VALIDATION_PASS)
	{
		printf("Validacion por Schema Fallido\n");
		return 0;
	}

	if (validateJsonImput(filenameJson) != VALIDATION_PASS)
	{
		printf("Validacion datos Json Fallido\n");
		return 0;
	}


	return 0;
}
