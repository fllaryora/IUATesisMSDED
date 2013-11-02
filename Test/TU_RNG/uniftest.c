#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "RNGs.h"

#define N 1000
#define M 100000

int main(int argc,char **argv)
{
   int i;
   double r,rmin=1e32,rmax=-1e32;
   double bins[N];
   double sum=0,sum2=0;

   for (i=0;i<N;i++)
      bins[i] = 0;

   //RandomInitialise(1802,9373);
   for (i=0;i<M;i++) {
      if (i % 1000 == 0)
         fprintf(stderr,"%d ",i);
      r = RandomUniform();
      sum += r;
      sum2 += (r*r);
      if (r >= 0 && r < 1)
         bins[(int)(r*N)]++;
   }
   
   for (i=0;i<N;i++)
      printf("%g %g\n",i/(double)N,bins[i]);
   
   fprintf(stderr,"Mean = %g\n",sum / M);
   fprintf(stderr,"Standard deviation = %g\n",(sum2 - sum*sum/M)/M);
}
