#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "RNGs.h"

int main(int argc,char **argv)
{
   int i;
   double r,rmin=1e32,rmax=-1e32;

   /* Generate 20000 random numbers */
   //RandomInitialise(1802,9373);
   for (i=0;i<20000;i++) {
      r = RandomUniform();
      if (r < rmin) rmin = r;
      if (r > rmax) rmax = r;
   }
   fprintf(stderr,"Numbers range from %g to %g\n",rmin,rmax);

   /*
      If the random number generator is working properly, 
      the next six random numbers should be:
         6533892.0  14220222.0  7275067.0
         6172232.0  8354498.0   10633180.0
   */
   for (i=0;i<6;i++)
      printf("%12.0lf ",4096.0 * 4096.0 * RandomUniform());
   printf("\n");
}
