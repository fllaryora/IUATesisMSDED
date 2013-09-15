#add PATH an libs on following script
export PATH=$PATH:/usr/lib64/mpich2/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/mpich2/lib

#mpich compilator
mpicc -o SIST test2.c 
