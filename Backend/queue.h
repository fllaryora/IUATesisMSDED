#ifndef _QUEUE_H_
#define _QUEUE_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#include "genericNode.h"



typedef enum{
	NOT_PROCESSESED, //estado inicial
	PROCESSESED // sacado de la lista de nodos a enviar
}statusNode;

extern void queueNode( const MPI_Comm , const  Queue*, const int );
extern void advancePhaseQueue(int *, int *, const MPI_Comm , int , const Queue *, const int);
extern int hasAvailableCombis(const int* , const Queue *);
extern void requestResponceCombis(const int* , const Queue *, const MPI_Comm , const int *);
extern void getDemandCombis(int* , const Queue *, const MPI_Comm );
extern void getFortunatedCombis(int* , const Queue *, const MPI_Comm , int* );
extern void generationPhaseQueue(int * , int * , const MPI_Comm );
#endif /* #ifndef _QUEUE_H_*/
