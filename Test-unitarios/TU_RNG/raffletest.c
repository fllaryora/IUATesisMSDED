#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "RNGs.h"
#include "scheduler.h"

#define N 20
#define M 100000

int main(int argc,char **argv)
{
   int i;
   int bins[N];
   int drawnNumbers[N];
   double sum=0;
   double sum2=0;

   for (i=0;i<N;i++)
      bins[i] = 0;
   for (i=0;i<N;i++)
      drawnNumbers[i] = i;
   
   for (i=0;i<M;i++) {
      Raffle(drawnNumbers, N-1);
      bins[drawnNumbers[N-1]]++;
      sum += (double)drawnNumbers[N-1];
      sum2 += ((double)drawnNumbers[N-1]*(double)drawnNumbers[N-1]);
   }
   
  for (i=0;i<N;i++)
     printf("%d\n", bins[i]);
   fprintf(stderr,"Mean = %g\n",sum / M);
   fprintf(stderr,"Standard deviation = %g\n",(sum2 - sum*sum/M)/M);
   
    sum=0;
    sum2=0;
   
  for (i=0;i<N;i++)
      bins[i] = 0;
   for (i=0;i<N;i++)
      drawnNumbers[i] = i;
   
   for (i=N-1;i >= 0;i--) {
      Raffle(drawnNumbers, i);
      bins[drawnNumbers[i]]++;
      
      sum += (double)drawnNumbers[i];
      sum2 += ((double)drawnNumbers[i]*(double)drawnNumbers[i]);
   }
   
  for (i=0;i<N;i++)
     printf("%d\n", bins[i]);
   fprintf(stderr,"Mean = %g\n",sum / N);
   fprintf(stderr,"Standard deviation = %g\n",(sum2 - sum*sum/N)/N);
       
}
