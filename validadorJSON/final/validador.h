#include "parson.h"
#include <stddef.h>
#include <wjelement.h>    

typedef enum{
	ERROR_OPEN_JSON,	/* json no existe */
	ERROR_OPEN_SCHEMA,	/* schema no existe */
	ERROR_READ_JSON,	/* ej: el json tenga solo una palabra */
	ERROR_READ_SCHEMA,  /* ej: el schema tenga solo una palabra */
	INVALID_JSON,		/* ej: sacando una coma al schema , esquema incompleto */
	INVALID_SCHEMA,		/* ej: sacando una coma al schema , esquema incompleto */
	VALIDATION_FAIL,	/* el json y el esquema son correctos pero el json no respeta el schema. ej: no incluir un elemento requerido o poner un number en lugar de un double*/
	VALIDATION_PASS		/* validacion correcta */
}VALIDATE;

typedef struct{
	char *distribution;
	double least;
	double highest;
	double constant;
	double mean;
	double variance;
	double lambda;
	double mode;
	double minimun;
	double maximun;
	double shapeAlpha;
	double shapeBeta;
	double shape;
	double escale;
	int seed;
}Delay;

typedef struct{
    int idNode;
    int resource;
    int *preceders; /*primer elemento va a ser la cantidad >=0*/
    int *followers;
    double fixedCost;
    double variableCost;
}Queue;

typedef struct{
    int idNode;
    int quantity;
    int *preceders;
    int *followers;
}Counter;

typedef struct{
	int idNode;
    int input;
	int output;
    int *preceders;
    int *followers;
    double *probabilisticBranch; /*primer elemento poner cero si no tiene elementos aunque sea double*/
}Function;

typedef struct{
    int idNode;
	int *preceders;
	int *followers;
    double *probabilisticBranch;
	Delay delay;
}Normal;

typedef struct{
	int idNode;
	int *preceders;
	int *followers;
    double *probabilisticBranch;
	Delay delay;
}Combi;

/*	Delay
	char *distribution; /*uniform/
	double least
	double highest
	int seed

	char *distribution; /*deterministic/
	double constant;

	char *distribution; /*normal/
	double mean;
	double variance;
	int seed

	char *distribution; /*exponential/
	double lambda;
	int seed

	char *distribution; /*triangular/
	double least
	double highest
	double mode
	integer seed

	char *distribution; /*beta/
	double minimun;
	double maximun;
	double shapeAlpha;
	double shapeBeta
	int seed;

	char *distribution; /*log-normal/
	double escale
	double shape
	double minimun
	int seed
*/
int					validateSchema(const char *filenameJson , const char *filenameSchema);
static void			schema_error(void *client, const char *format, ...);
static WJElement	schema_load(const char *name, void *client, const char *file, const int line);
int validateJsonImput(const char *filenameJson);
void getArray(JSON_Object *object, const char *arregloJson,const char *atributeJson ,int** arreglo, int* m);
void getArrayInArray(JSON_Object * objectJson,const char *arrayJson,int pos,const char *arrayJsonIn, int** arreglo, int* m);
int countArrayInclude(int **array,int sizeArray,int **arrayFull, int sizeArrayFull);
int repeatArrays(int **array1 ,int sizeArray1, int **array2,int sizeArray2, int **array3, int sizeArray3, int **array4, int sizeArray4, int **array5, int sizeArray5,int** array, int* sizeArray);

