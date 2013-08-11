//compilo con $mpicc sender.c -o xsen
//ejecuto con  $LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib mpirun -np 3 xsen
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define N 10
#define INDEX(i,j) (i)*N+(j)
//el espacio es toroidal
//la matriz debe ser cuadrada
//para 2 a N procesos

int generation(int* a, int* b, int cantidadDeFilas, int* filaArriba, int* filaAbajo);
int neighbours(int* a, int i, int j);

void load_world(int* matrix){
//	int i,j;
//cualquiercosa para probar el la transferencia.
//	for(i = 0 ; i < N; i++)
//		for(j = 0; j < N ; j++)
//			 matrix[INDEX(i , j)] = i;

//El glider
	matrix[INDEX(6,0)] = matrix[INDEX(6,1)] = matrix[INDEX(6,2)] = 1;
	matrix[INDEX(7,2)] = matrix[INDEX(8,1)] = 1;

//estructura estable
//	matrix[INDEX(6,0)] = matrix[INDEX(6,1)] = 1;
//	matrix[INDEX(7,0)] = matrix[INDEX(7,1)] = 1;

}

void print_world(int* matrix){
  int i, j;
  for (i= N-1 ; i >= 0; i--){
    for (j=0 ; j < N; j++){
      printf(matrix[INDEX(i,j)]?"o":"x");
    }
    printf("\n");
  }
}

int main(int argc, char* argv[]){

//datos comunes a todos 
	int my_rank;   /* my id on communicator*/
	int p;         /* number of processes */
	int base, offset;
	int cantidadDeFilas; /* Cantidad de Filas que me toca computar*/ 
	int* a; 
	int* b; 
	int* filaArriba ;
	int* filaAbajo ;
	int* aux;
	int i,j,k;
	MPI_Status status;
	MPI_Request req[2];
	MPI_Status sts[2];
	int arriba ;
	int abajo ; 
	int t;
	int hayCambio, AllCambio; 
	//datos del rank=0 
	int* matfull;
	int* displays ;
	int* cantidad ;
	int other_rank;
	int filaInicial; /* Fila que toca empieza desde 0 a N-1*/ 
	int other_cantRows;

	MPI_Init(&argc, &argv);
 
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if( p < 2 || p>N ){ MPI_Finalize(); return 0; }

	base = N/p;
	offset = N%p;
	cantidadDeFilas = base;
	if(my_rank <= (offset-1) )
		cantidadDeFilas++;


	if(my_rank == 0){
		matfull = calloc(N*N, sizeof(int));
		load_world(matfull);
		print_world(matfull);
		printf("\n");
		printf("\n");
		displays = calloc(p, sizeof(int));
		cantidad = calloc(p, sizeof(int));

		for(other_rank = 0 ; other_rank < p ; other_rank++ ){
			filaInicial = base* other_rank;
			if(other_rank <= offset)
				filaInicial += other_rank;
			else
				filaInicial += offset;

			other_cantRows = base;
			if(other_rank <= (offset-1) )
				other_cantRows++;


			displays[other_rank] = INDEX( filaInicial , 0 );
			cantidad[other_rank] = other_cantRows*N;
		}

	}//fin Rank=0

	a = calloc(cantidadDeFilas*N, sizeof(int));
	b = calloc(cantidadDeFilas*N, sizeof(int));
	filaArriba = calloc( N, sizeof(int));
	filaAbajo = calloc( N, sizeof(int));
	arriba = ((my_rank-1+p) %p); //rango del de arriba
	abajo = ((my_rank+1) %p); //rango del de abajo


	MPI_Scatterv( matfull , cantidad , displays, MPI_INT,
 					 a, cantidadDeFilas*N , MPI_INT,
					 0, MPI_COMM_WORLD);

	for(t=0; t < 9; t++){//bucle en el tiempo
		hayCambio = 0;

				MPI_Isend( a, N, MPI_INT, //datos
							 arriba , 0, MPI_COMM_WORLD , &req[0]); 

				MPI_Isend( &a[INDEX( cantidadDeFilas-1 , 0 )], N, MPI_INT, //datos
							 abajo , 0, MPI_COMM_WORLD , &req[1]);

				MPI_Recv( filaAbajo , N, MPI_INT,
						   abajo, 0, MPI_COMM_WORLD , &status);

				MPI_Recv( filaArriba , N, MPI_INT,
						   arriba, 0, MPI_COMM_WORLD , &status);

	
//				printf(" Soy %d\n",my_rank);
//				//for(i = cantidadDeFilas-1 ; i >= 0; i--){
//				for(i = 0 ; i < N; i++)
//					printf("%d ", filaArriba[INDEX(0 , i)] );
//				printf("\n");
//				for(i = 0 ; i < cantidadDeFilas; i++){
//					for(j = 0; j < N ; j++)
//						printf("%d ", a[INDEX(i , j)] );
//					printf("\n");
//				}
//				for(i = 0 ; i < N; i++)
//					printf("%d ", filaAbajo[INDEX(0 , i)] );
//				printf("\n");


				MPI_Waitall(2, req, sts);


				hayCambio = generation( a , b , cantidadDeFilas, filaArriba, filaAbajo);

				MPI_Gatherv(b, cantidadDeFilas*N, MPI_INT,
						      matfull, cantidad, displays, MPI_INT,
						   	0, MPI_COMM_WORLD);
				if(my_rank == 0){
					print_world(matfull);
					printf("\n");
					printf("\n");
				}

				MPI_Allreduce(&hayCambio, &AllCambio, 1,
		            MPI_INT , MPI_SUM , MPI_COMM_WORLD);
				aux = a;
				a = b;
				b = aux;
			if(AllCambio == 0) break;
	}//fin bucle en el tiempo

	free(a);
	free(b);
	free(filaArriba);
	free(filaAbajo);
	if(my_rank == 0){	free(matfull); }
	MPI_Finalize();

	return 0;
}

int generation(int* a, int* b, int cantidadDeFilas, int* filaArriba, int* filaAbajo){
	int i,j,n;
	int changed = 0;

	for (i = 1; i < cantidadDeFilas-1; i++){
		for(j=0; j<N; j++){
			n = neighbours(a,i,j);

			if (a[INDEX(i,j)]){
				b[INDEX(i,j)] = (n==2 || n==3);
			} 
			else {
				b[INDEX(i,j)] = (n==3);
			}
			if (a[INDEX(i,j)]!=b[INDEX(i,j)])
				changed = 1;
		}
	}

//	para i = 0;
	for(j=0; j<N; j++){
		n = filaArriba[ (j+N-1)%N ] + filaArriba[ (j+1)%N ] + filaArriba[ j ] ;
		n += a[INDEX( 0 ,(j+N-1)%N)] + a[INDEX( 0 ,(j+1)%N)];
		n += a[INDEX( 1,(j+N-1)%N)] + a[INDEX( 1 ,j)] + a[INDEX( 1,(j+1)%N)];
		if (a[INDEX(0,j)]){
			b[INDEX(0,j)] = (n==2 || n==3);
		} 
		else {
			b[INDEX(0,j)] = (n==3);
		}
		if (a[INDEX(0,j)]!=b[INDEX(0,j)])
			changed = 1;
	}
//	para i = cantidadDeFilas-1;
	i = cantidadDeFilas-1;
	for(j=0; j<N; j++){
		n = a[INDEX((i-1),(j+N-1)%N)] + a[INDEX((i-1),j)] + a[INDEX((i-1),(j+1)%N)];
		n += a[INDEX(i,(j+N-1)%N)] + a[INDEX(i,(j+1)%N)];
		n += filaAbajo[ (j+N-1)%N ] + filaAbajo[ (j+1)%N ] + filaAbajo[ j];

		if (a[INDEX(i,j)]){
			b[INDEX(i,j)] = (n==2 || n==3);
		} 
		else {
			b[INDEX(i,j)] = (n==3);
		}
		if (a[INDEX(i,j)]!=b[INDEX(i,j)])
			changed = 1;
	}

	return changed;
}


int neighbours(int* a, int i, int j){
  return a[INDEX(i,(j+N-1)%N)] + a[INDEX(i,(j+1)%N)] 
		+ a[INDEX((i-1),(j+N-1)%N)] + a[INDEX((i-1),j)] + a[INDEX((i-1),(j+1)%N)] 
		+ a[INDEX((i+1),(j+N-1)%N)] + a[INDEX((i+1),j)] + a[INDEX((i+1),(j+1)%N)];
}

