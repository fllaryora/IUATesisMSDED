COMPILER="gcc"
GCCARGS="-Wall -O0 -g -std=c99"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
$COMPILER -Wall $GCCARGS -c parson.c
$COMPILER -Wall $GCCARGS -c jsonHelper.c
$COMPILER -Wall $GCCARGS jsonHelperTest.c -o testJson
rm *.o
valgrind ./testJson
