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
   int bins[N+1];
   double sum=0,sum2=0;

   for (i=0;i<N;i++)
      bins[i] = 0;

   //RandomInitialise(1802,9373);
   for (i=0;i<M;i++) {
      r = RandomTriangular(0.0,1000.0, 500.0);
      sum += r;
      sum2 += (r*r);
      int entero = (int)r;
      /* printf("%g\n",r); */
      if (r >= 0.0 && r <  1000.0)
		bins[entero]++;
   }
   for (i=0;i<N;i++)
      printf("%d %d\n",i,bins[i]);
   
   fprintf(stderr,"Mean = %g\n",sum / M);
   fprintf(stderr,"Standard deviation = %g\n",sqrt((sum2 - sum*sum/M)/M));

}
