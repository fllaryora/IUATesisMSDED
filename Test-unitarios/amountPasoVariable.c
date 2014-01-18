#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
// gcc -lm lala.c -o lala

int main(){
    float b=123456.1415926535897932384626433832795;
    double c=123456.1415926535897932384626433832795;
    double d = floor(c*1000)/1000.0;
    printf("Assigned value for Pi:3.1415926535897932384626433832795\n\n");
    printf("Pi as float:%.20f\n\n",b);
    printf("Pi as double:%.20lf\n\n",c);
    printf("precision float %d double %d\n", FLT_DIG, DBL_DIG);

    
    printf("Pi as double:%.20lf\n\n",d);

        int amount[10] = {8,8,9,9,2,2,5,5,5,4};
        int paso[5] = {1,1,2,2,3};
        double amountAverage = 0.0;
        int amountAnterior = 0;
        int i = 0; 
        for(i = 1; i < 10 ; i++){
            amountAverage =  (amountAverage*(i-1) + amount[i] )/ (double)i;
            printf( "%lf\n", amountAverage);
        }

        printf("------------------------------------\n");
        amountAverage = 0.0; 
        amountAverage =  amount[1] ;
        printf( "%lf\n", amountAverage);
        amountAverage = (amountAverage*(2-1) + amount[2] )/ (double)2;
        printf( "%lf\n", amountAverage);

        amountAverage = (amountAverage*(4-2) + amount[4] + amount[2])/ (double)4;
        printf( "%lf\n", amountAverage);
        amountAverage = (amountAverage*(6-2) + amount[6] + amount[4] )/ (double)6;
        printf( "%lf\n", amountAverage);
        amountAverage = (amountAverage*(9-3) + amount[9] + amount[6] + amount[6])/ (double)9;
        printf( "%lf\n", amountAverage);

        printf("------------------------------------\n");
        
        amountAnterior = 0;
        int pasoActual;
        int pasoAcumulado = 0;
        double amountAverageAnterior = 0.0;
        for(i = 0; i < 5 ; i++){
            pasoActual = paso[i];
            pasoAcumulado += pasoActual;
            amountAverageAnterior = amountAverage*(pasoAcumulado-pasoActual);
            amountAverage =  (amountAverageAnterior + amount[pasoAcumulado] + amountAnterior*(pasoActual-1) )/(double)pasoAcumulado;
            amountAnterior = amount[pasoAcumulado];
             printf( "%lf\n", amountAverage);
        }
        return 0;
}
