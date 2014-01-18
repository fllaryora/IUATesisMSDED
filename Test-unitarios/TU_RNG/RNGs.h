#ifndef _RNG_S_H_
#define _RNG_S_H_

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

extern void   RandomInitialise(int,int);
extern double RandomUniform(void);
extern double RandomTriangular(double,double,double );
extern double RandomGaussian(double,double);
extern double RandomLogNormal(double,double);
extern double RandomLogNormalWithMinimun(double,double,double);
extern double RandomExponential(double );

extern double RandomBeta(double  ,double ,double  ,double );
extern double RandomGamma(double  ,double ,double);
extern double exponential( double a, double b );
extern int    RandomInt(int,int);
extern double RandomDouble(double,double);
extern int SeedGenerator(int );
extern double RandomGAmmaDecimal(int  ,double );
#endif /* #ifndef _RNG_S_H_*/
