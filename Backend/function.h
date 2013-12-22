#ifndef _FUNCTION_H_
#define _FUNCTION_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#include "genericNode.h"
#include "RNGs.h"

extern void functionNode( const MPI_Comm ,  const  Function *, const int);
extern void advancePhaseFunction(int * inputResource, int* outputResource, const Function *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, RngInstance* rngProbabilisticBranch);
extern void generationPhaseFunction(int*  , int* , int* ,  const MPI_Comm  , const Function * );

#endif /* #ifndef _FUNCTION_H_*/
