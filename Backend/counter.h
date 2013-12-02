#ifndef _COUNTER_H_
#define _COUNTER_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#include "genericNode.h"


extern void counterNode( const MPI_Comm ,  const  Counter *, const int);
extern void advancePhaseCounter(int * , int* , const Counter *, const MPI_Comm , const int ,const int , PrinterCounter *);
extern void generationPhaseCounter(int* , int* ,  const MPI_Comm commNodes);

#endif /* #ifndef _COUNTER_H_*/
