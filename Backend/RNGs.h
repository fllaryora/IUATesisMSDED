#ifndef _RNG_S_H_
#define _RNG_S_H_

#include <time.h>
#include <stdio.h>
#include <math.h>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef struct{
  double uList[97];
  double coeficient;
  double coeficientD;
  double coeficientM;
  int isInitialise;
  int integer97;
  int jinteger97;
}RngInstance;

extern void   RandomInitialise(RngInstance*, int, int);
extern double RandomUniform(RngInstance*);
extern double RandomTriangular(RngInstance*, const double, const double, const double );
extern double RandomGaussian(RngInstance*, const double, const double);
extern double RandomLogNormal(RngInstance*, const double, const double);
extern double RandomLogNormalWithMinimun(RngInstance * , const double , const double , const double );
extern double RandomExponential(RngInstance*, const double );
extern double RandomBeta(RngInstance*, const double , const double );
extern double RandomBetaInteger(RngInstance*, const int, const int );
extern double RandomBetaWithMinimunAndMaximun(RngInstance*, const double, const double, const double, const double );
extern double RandomBetaIntegerWithMinimunAndMaximun(RngInstance*, const int, const int, const double, const double );
//extern double RandomGamma(double  ,double );
extern double RandomGammaInteger(RngInstance*, const int, const double );
extern double RandomNormal(RngInstance*,const double, const double );
extern int    RandomInt(RngInstance*, const int, const int);
extern double RandomDouble(RngInstance*, const double, const double);
extern int SeedGenerator(const int );

#endif /* #ifndef _RNG_S_H_*/
