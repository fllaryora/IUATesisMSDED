
#include <stdio.h>

void cambioArreglo(int** arreglo, int* m){
	*m = 5;
	
	*arreglo = (int*)malloc(sizeof(int) * 5);

}

int main(int argc, char **argv){
  int cantidad = 4;
  int* arreglo;
  
  arreglo[0] = 6;
  cambioArreglo( arreglo, &cantidad);

  int n;
  for (n = 0; n < cantidad ; n++)
	printf("%d ",arreglo[n]);
	
  return 0;
}

