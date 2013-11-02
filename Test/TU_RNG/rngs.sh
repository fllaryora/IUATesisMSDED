#compilo los generadores matematicos
gcc -lm -c RNGs.c

#compilo las funciones del scheduler
gcc RNGs.o -lm -c scheduler.c

#muestra numeros aleatoreos
gcc RNGs.o -lm -o RNG randomtest.c 

#valida que la media sea 0.5 y que la desviacion estandar sea 0.
gcc RNGs.o -lm -o UNI uniftest.c

#valida que la media sea cercana a la moda.
gcc RNGs.o -lm -o TRI triangulartest.c 

#valida que la media sea 1 y que la desviacion estandar sea 0.5.
gcc RNGs.o -lm -o GAU gaustest.c 

#valida que la desviacion estandar sea 1;
gcc RNGs.o -lm -o EXP exponentialtest.c

#valida que el promedio sea igual a la raiz cuadrada de e;
gcc RNGs.o -lm -o LNO lognormaltest.c


#valida que la media sea alfa sobre beta
gcc RNGs.o -lm -o GMM gammatest.c


#valida que la media sea alfa sobre alfa mas beta
gcc RNGs.o -lm -o BTA betatest.c

#valida que la media sea 9.5 y que la variacion estandar sea 33.25.
gcc RNGs.o  scheduler.o -lm -o RAF raffletest.c 

#elimino el objetos
rm *.o
