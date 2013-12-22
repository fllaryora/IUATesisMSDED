#ifndef _COMBI_H_
#define _COMBI_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif
#include "genericNode.h"
#include "RNGs.h"

extern void combiNode( const MPI_Comm ,  const  Combi *, const int);
extern void advancePhaseCombi(int * inputWorktask, int* outputWorktask, const Combi *initialStatus, const MPI_Comm commNodes, const int mpiProcesses,const int isPrima, RngInstance* rngProbabilisticBranch, const int fileDescriptor);
extern int hasQueueResources( const Combi *, const MPI_Comm , const int );
extern void resourcesNoDemand( const Combi *, const MPI_Comm , const int);
extern void resourcesDemand( const Combi *, const MPI_Comm , const int );
extern void resourcesSend(const Combi *initialStatus, const MPI_Comm commNodes, int* worktaskInOutput, RngInstance* rngProbabilisticBranch , const int fileDescriptor);
extern void finishCombi(const int , const MPI_Comm  ,const int* , const int , const int );
extern int allTransactionBegin(const MPI_Comm  ,const Combi *,  const int );
extern void setAllRollback(const Combi *, const MPI_Comm , const int );
extern void setAllCommit(const Combi *, const MPI_Comm , const int );

extern void generationPhaseCombi(int* , int* ,int*, const MPI_Comm , Worktask *,  const Combi *, PrinterActivity* ,RngInstance* );
#endif /* #ifndef _COMBI_H_*/
