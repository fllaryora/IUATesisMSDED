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

typedef enum{
	DIST_UNIFORM,
	DIST_DETERMINISTIC,
	DIST_NORMAL,
	DIST_EXPONENTIAL,
	DIST_TRIANGULAR,
	DIST_BETA,
	DIST_LOG_NORMAL
}DISTRIBUTION;

typedef struct{
	int distribution; //
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
    int countPreceders;
    int *preceders;
    int countFollowers;
    int *followers;
    double fixedCost;
    double variableCost;
}Queue;

typedef struct{
    int idNode;
    int quantity;
    int countPreceders;
    int *preceders;
    int countFollowers;
    int *followers;
}Counter;

typedef struct{
	int idNode;
    int input;
	int output;
    int countPreceders;
    int *preceders;
    int countFollowers;
    int *followers;
    int countProbabilisticBranch;
    double *probabilisticBranch;
}Function;

typedef struct{
    int idNode;
    int countPreceders;
    int *preceders;
    int countFollowers;
	int *followers;
    int countProbabilisticBranch;
    double *probabilisticBranch;
	Delay delay;
}Normal;

typedef struct{
	int idNode;
    int countPreceders;
    int *preceders;
    int countFollowers;
	int *followers;
    int countProbabilisticBranch;
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

/* UTILIDAD */

static void			schema_error(void *client, const char *format, ...);
static WJElement	schema_load(const char *name, void *client, const char *file, const int line);

/* VALIDACIONES*/

int	validateSchema(const char *filenameJson , const char *filenameSchema);
int validateJsonImput(const char *filenameJson);

void getArray(JSON_Object *object, const char *arregloJson,const char *atributeJson ,int** arreglo, int* m);
void getArrayInArray(JSON_Object * objectJson,const char *arrayJson,int pos,const char *arrayJsonIn, int** arreglo, int* m);
int countArrayInclude(int **array,int sizeArray,int **arrayFull, int sizeArrayFull);
int repeatArrays(int **array1 ,int sizeArray1, int **array2,int sizeArray2, int **array3, int sizeArray3, int **array4, int sizeArray4, int **array5, int sizeArray5,int** array, int* sizeArray);

/* ESTRUCTURA */

void getQueues(const char *filenameJson , Queue **queues, int *queuesCount);
void getCounters(const char *filenameJson , Counter **counters, int *counterCount);
void getFunctions(const char *filenameJson , Function **functions, int *functionCount);
void getNormals(const char *filenameJson , Normal **normals, int *normalCount);
void getCombis(const char *filenameJson , Combi **combis, int *combiCount);

void printQueue(Queue queue);
void printCombi(Combi combi);
