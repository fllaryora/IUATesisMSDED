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

extern int SeedGenerator(const int );
extern void   RandomInitialise(RngInstance*, int, int);
extern double RandomUniform(RngInstance*);

extern int    RandomInt(RngInstance*, const int, const int);
extern double RandomDouble(RngInstance*, const double, const double);

extern double RandomTriangular(RngInstance*, const double, const double, const double );

extern double RandomGaussian(RngInstance*, const double, const double);

extern double RandomLogNormal(RngInstance*, const double, const double);

extern double RandomLogNormalWithMinimun(RngInstance * , const double , const double , const double );

extern double RandomExponential(RngInstance*, const double );

extern double RandomNormal(RngInstance*,const double, const double );

extern double RandomGamma(RngInstance * , double , double , double );

extern double RandomBeta(RngInstance * , double  ,double , double  ,double  );
#endif /* #ifndef _RNG_S_H_*/
