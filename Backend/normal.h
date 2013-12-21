#ifndef _NORMAL_H_
#define _NORMAL_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#include "genericNode.h"
#include "RNGs.h"

extern void normalNode( const MPI_Comm ,  const  Normal *, const int );
extern void advancePhaseNormal(int * inputWorktask, int* outputWorktask, const Normal *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, RngInstance* rngProbabilisticBranch );
extern void generationPhaseNormalPrima(int* , int* , int*  ,const MPI_Comm , Worktask *,  const Normal *, const int , PrinterActivity* ,RngInstance *);

#endif /* #ifndef _NORMAL_H_*/
