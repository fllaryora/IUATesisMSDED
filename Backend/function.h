#ifndef _FUNCTION_H_
#define _FUNCTION_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#include "genericNode.h"


extern void functionNode( const MPI_Comm ,  const  Function *, const int);
extern void advancePhaseFunction(int * , int* , const Function *, const MPI_Comm , const int ,const int );


#endif /* #ifndef _FUNCTION_H_*/