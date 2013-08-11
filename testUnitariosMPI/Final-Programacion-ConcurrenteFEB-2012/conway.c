#include <stdio.h>
#include <stdlib.h>




#define N 10
#define INDEX(i,j) (i)*N+(j)


void load_world(int* matrix){
  matrix[INDEX(3,0)] = matrix[INDEX(3,1)] = matrix[INDEX(3,2)] = 1;
}

void print_world(int* matrix){
  int i, j;
  for (i=0; i < N; i++){
    for (j=0; j<N; j++){
      printf(matrix[INDEX(i,j)]?"*":".");
    }
    printf("\n");
  }
}

int neighbours(int* a, int i, int j){
  return a[INDEX(i,(j+N-1)%N)] + a[INDEX(i,(j+1)%N)] + a[INDEX((i+N-1)%N,(j+N-1)%N)] + a[INDEX((i+N-1)%N,j)] + a[INDEX((i+N-1)%N,(j+1)%N)] + a[INDEX((i+1)%N,(j+N-1)%N)] + a[INDEX((i+1)%N,j)] + a[INDEX((i+1)%N,(j+1)%N)];
}

int generation(int* a, int* b){
  int i,j;
  int changed = 0;
  for (i=0; i< N; i++){
    for(j=0; j<N; j++){
      int n = neighbours(a,i,j);
      if (a[INDEX(i,j)]){
	b[INDEX(i,j)] = (n==2 || n==3);
      } else {
	b[INDEX(i,j)] = (n==3);
      }
      if (a[INDEX(i,j)]!=b[INDEX(i,j)])
	changed = 1;
    }
  }
  return changed;
}

int main(){
  int *a = calloc(N*N, sizeof(int));
  int *b = calloc(N*N, sizeof(int));
  int *aux;
  int i;
  load_world(a);
  print_world(a);
  for(i=0; i<5; i++){
    generation(a,b);
    print_world(b);
    aux=a;
    a=b;
    b=aux;
  }
  return 0;
}
