COMPILER="gcc"
GCCARGS="-O3 --std=c99 -I/usr/local/include -L/usr/local/lib"
RUNFORESTRUN="mpirun -np $2"
	
#Once you’ve built and installed the libraries, you can compile the sample with gcc like so:

#gcc --std=c99 -I/usr/local/include -L/usr/local/lib -o example docs/examples/example.c -lmongoc



#echo "$COMPILER -Wall $GCCARGS -c scheduler.c"
#$COMPILER -Wall $GCCARGS -c scheduler.c

echo "$COMPILER -Wall $GCCARGS -o Bootstrap bootstrap.c -lmongoc"
$COMPILER -Wall $GCCARGS -o Bootstrap bootstrap.c -lmongoc

rm *.o

echo "$RUNFORESTRUN ./Bootstrap"
#$RUNFORESTRUN ./Engine
