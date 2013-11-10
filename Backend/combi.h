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
extern void advancePhaseCombi(int * , int * , const Counter *, const MPI_Comm , const int ,const int );
extern int hasQueueResources( const Counter *, const MPI_Comm );
extern void resourcesNoDemand( const Counter *, const MPI_Comm );
extern void resourcesDemand( const Counter *, const MPI_Comm );
extern void resourcesSend( const Counter *, const MPI_Comm , int );
extern void finishCombi(const int , const MPI_Comm  ,const int , const int );
extern int allTransactionBegin(const MPI_Comm  ,const Counter *);
extern void setAllRollback(const Counter *, const MPI_Comm );
extern void setAllCommit(const Counter *, const MPI_Comm );


#endif /* #ifndef _COMBI_H_*/
