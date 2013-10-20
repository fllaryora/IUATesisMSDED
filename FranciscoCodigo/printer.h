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
extern void doDeltaT(int , const double );

extern void doQueue(int , const int , const int , const int , const int , const double , const int ,  const int ,  const double ,  const double );
extern void doCounter(int , const int , const int , const int , const double  );
extern void doFunction(int , const int , const int );
#define doNormal(A , B , C, D, E, F, G, H, I )	doActivity(A , B , C, D, E, F, G, H, I );
#define doCombi(A , B , C, D, E, F, G, H, I )	doActivity(A , B , C, D, E, F, G, H, I );
extern void doActivity(int , const int , const int , const double* , const double* , const int , const double , const int , const int  );

#endif /* #ifndef _PRINTER_H_*/
