#ifndef _NORMAL_H_
#define _NORMAL_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#include "genericNode.h"


extern void normalNode( const MPI_Comm ,  const  Normal *, const int, const int );
extern void advancePhaseNormal(int * , int* , const Normal *, const MPI_Comm , const int ,const int , const int );
extern void generationPhaseNormalPrima(int* , int* , int*  ,const MPI_Comm , Worktask *,  const Normal *, const int );

#endif /* #ifndef _NORMAL_H_*/
