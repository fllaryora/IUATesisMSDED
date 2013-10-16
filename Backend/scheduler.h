#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

extern void scheduler(unsigned long , const MPI_Comm  , const int * const  , const int , const int );
#endif /* #ifndef _SCHEDULER_H_*/
