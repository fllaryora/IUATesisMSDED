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
extern double RandomBeta(double  ,double );
extern double RandomBetaInteger(int  ,int );
extern double RandomBetaWithMinimunAndMaximun(double  ,double ,double  ,double );
extern double RandomBetaIntegerWithMinimunAndMaximun(int  ,int,double  ,double  );
//extern double RandomGamma(double  ,double );
extern double RandomGammaInteger(int  ,double );
extern double RandomNormal(double ,double );
extern int    RandomInt(int,int);
extern double RandomDouble(double,double);
extern int SeedGenerator(int );
#endif /* #ifndef _RNG_S_H_*/
