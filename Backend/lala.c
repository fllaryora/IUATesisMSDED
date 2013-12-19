#include<stdio.h>
#include<stdlib.h>

int main(){
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
