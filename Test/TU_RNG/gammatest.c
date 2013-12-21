#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "RNGs.h"

#define N 1000
#define M 50

// int main(int argc,char **argv)
// {
//    int i;
//    double r,rmin=1e32,rmax=-1e32;
//    int bins[N+1];
//    double sum=0,sum2=0;

//    for (i=0;i<N;i++)
//       bins[i] = 0;

//    //RandomInitialise(1802,9373);
//    for (i=0;i<M;i++) {
//       r = RandomGammaInteger(10, 2.0);
//       sum += r;
//       printf("%g \n",r);
//       sum2 += (r*r);
//       int entero = (int)r;
//       /* printf("%g\n",r); */
//       if (r >= 0.0 && r <  1000.0)
// 		bins[entero]++;
		
//    }
//   // for (i=0;i<N;i++)
//     //  printf("%d %d\n",i,bins[i]);
   
//    fprintf(stderr,"Mean = %g\n",sum / M);
//    fprintf(stderr,"Standard deviation = %g\n",sqrt((sum2 - sum*sum/M)/M));

// }


int main(int argc,char **argv)
{
   int i;
   double c;
   double sum = 0.0;
   //RandomInitialise(1802,9373);
   for (i=0;i<M;i++) {
      c =  RandomGamma( 0.0, 1.0 , 0.6 );
      sum += c;
   }
   printf("la media de gamma es  a+ b*c (en este caso 2) %lf\n", sum/M);
   /*++
este caso 2) 1.901096
[francisco@lechuza TU_RNG]$ ./GMM
la media de gamma es  a+ b*c (en este caso 2) 2.273964
[francisco@lechuza TU_RNG]$ ./GMM
la media de gamma es  a+ b*c (en este caso 2) 1.911473


   +*/
}
