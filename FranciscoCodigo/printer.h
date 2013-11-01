#ifndef _PRINTER_H_
#define _PRINTER_H_


#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

extern void printer(void);
extern void openBrace(int );
extern void closeBrace(int );
extern void openBracket(int );
extern void closeBracket(int );
extern void separeElement(int );
extern void putLabel(int , const char* );
extern void putInteger(int , const int );
extern void putDouble(int , const double );
extern void putArrayDouble(int , const double* , int );
extern void putArrayInteger(int , const int* , int );
extern void doSummaryReport(int , const double , const int , const int  );
extern void doDeltaT(int , const double , const int , const int , const int , const int , const int );

extern void doQueue(int , const int , const int , const int , const int , const double , const int ,  const int ,  const double ,  const double );
extern void doFinalQueue(int , const int , const double , const double );
extern void doCounter(int , const int , const int , const int , const double  );
extern void doFinalCounter(int , const int , const int );
extern void doFunction(int , const int , const int );
extern void doActivity(int , const int , const int , const double* , const double* , const int , const double , const int , const int  );

#define GetFinalCounterStruct(STRU)	MPI_Recv(STRU, sizeof(PrinterFinalCounter), MPI_BYTE, MPI_ANY_SOURCE, COUNTER_FINAL_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE)
#define GetFinalQueueStruct(STRU)	MPI_Recv(STRU, sizeof(PrinterFinalQueue), MPI_BYTE, MPI_ANY_SOURCE, QUEUE_FINAL_REPORT , MPI_COMM_WORLD, MPI_STATUS_IGNORE)
#endif /* #ifndef _PRINTER_H_*/
