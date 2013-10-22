#ifndef _PRINTER_H_
#define _PRINTER_H_
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
extern void doSummaryReport(int );
extern void doDeltaT(int , const double , const int , const int , const int , const int , const int );

extern void doQueue(int , const int , const int , const int , const int , const double , const int ,  const int ,  const double ,  const double );
extern void doCounter(int , const int , const int , const int , const double  );
extern void doFunction(int , const int , const int );
#define doNormal(A , B , C, D, E, F, G, H, I )	doActivity(A , B , C, D, E, F, G, H, I );
#define doCombi(A , B , C, D, E, F, G, H, I )	doActivity(A , B , C, D, E, F, G, H, I );
extern void doActivity(int , const int , const int , const double* , const double* , const int , const double , const int , const int  );

#define GetNormalStruct(STRU, STTUS)	GetActivityStruct(STRU, sizeof(PrinterActivity), NORMAL_REPORT, STTUS)
#define GetCombiStruct(STRU, STTUS)	GetActivityStruct(STRU, sizeof(PrinterActivity), COMBI_REPORT, STTUS)
#define GetCounterStruct(STRU)	GetNodeStruct(STRU, sizeof(PrinterCounter), COUNTER_REPORT)
#define GetQueueStruct(STRU)	GetNodeStruct(STRU, sizeof(PrinterQueue), QUEUE_REPORT)
#define GetFunctionStruct(STRU)	GetNodeStruct(STRU, sizeof(PrinterFunction), FUNCTION_REPORT)

#define GetCombiDalaysStruct(DLY, SIZE, NODE_ID)	GetDelayActivityArrayStruct(DLY, SIZE, COMBI_REPORT, NODE_ID)
#define GetNormalDalaysStruct(DLY, SIZE, NODE_ID)	GetDelayActivityArrayStruct(DLY, SIZE, NORMAL_REPORT, NODE_ID)

#endif /* #ifndef _PRINTER_H_*/
