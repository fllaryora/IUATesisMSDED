#ifndef _COMBI_H_
#define _COMBI_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#include "genericNode.h"


extern void combiNode( const MPI_Comm ,  const  Combi *, const int);
extern void advancePhaseCombi(int * , int * , const Combi *, const MPI_Comm , const int ,const int );
extern int hasQueueResources( const Combi *, const MPI_Comm );
extern void resourcesNoDemand( const Combi *, const MPI_Comm );
extern void resourcesDemand( const Combi *, const MPI_Comm );
extern void resourcesSend( const Combi *, const MPI_Comm , int * );
extern void finishCombi(const int , const MPI_Comm  ,const int* , const int );
extern int allTransactionBegin(const MPI_Comm  ,const Combi *);
extern void setAllRollback(const Combi *, const MPI_Comm );
extern void setAllCommit(const Combi *, const MPI_Comm );


#endif /* #ifndef _COMBI_H_*/
