COMPILER="gcc"
GCCARGS="-Wall -O0 -g -std=c99"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
$COMPILER -Wall $GCCARGS -c parson.c
$COMPILER -Wall $GCCARGS -c jsonHelperN.c
$COMPILER -Wall $GCCARGS -lwjelement -lwjreader parson.o jsonHelperN.o jsonHelperTest.c -o testJson

valgrind -v ./testJson
rm *.o
