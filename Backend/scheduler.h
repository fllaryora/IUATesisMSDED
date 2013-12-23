#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

extern void scheduler(unsigned long watchdog, const MPI_Comm commNodes , CycleValidator* targets, const int mpiProcesses, const int counterNodes);
#endif /* #ifndef _SCHEDULER_H_*/
