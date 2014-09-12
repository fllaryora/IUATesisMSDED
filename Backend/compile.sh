#!/bin/bash

#para poder ejecutarlo desde cualquier lado
if [ -z ${BOTQUEUE_HOME+x} ]
then
	echo "BOTQUEUE_HOME var is unset"
	echo "end of compile"
	exit 0
else
	echo "BOTQUEUE_HOME existe"
fi

if [ -z ${BOTQUEUE_CODE+x} ]
then
	BOTQUEUE_CODE=/tmp/Engine
else
	echo "BOTQUEUE_CODE existe"
fi

cd $BOTQUEUE_HOME


MAXSTEP=1.0
MINSTEP=0.01
REALSTEP=1.0
MINPARAMS=2
COMPILER=" "
GCCARGS=" "
RUNFORESTRUN=" "


if [ -e "$BOTQUEUE_CODE" ]
then
	if [ -f "$BOTQUEUE_CODE" ]
	then
		echo "rm $BOTQUEUE_CODE"
		rm $BOTQUEUE_CODE
	else
		echo "no hay $BOTQUEUE_CODE"
	fi
else
	echo "no hay $BOTQUEUE_CODE"
fi


if [ $# -lt "$MINPARAMS" ]
then
	echo
	echo "This script needs at least $MINPARAMS command-line arguments!"
	echo "first argument TEST or PRODUCTION"
	echo "second argument node type or all"
	echo "./compile.sh TEST SCHEDULER"
	echo "./compile.sh TEST RAFFLER"
	echo "./compile.sh TEST PRINTER"
	echo "./compile.sh TEST GENERIC"
	echo
	exit 0
fi

if [ $1 == "PRODUCTION" ]
then
	echo
	echo "compiling environment PRODUCTION"
	COMPILER="mpicc"
	export PATH=$PATH:/usr/lib64/mpich2/bin
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/mpich2/lib
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
	#re='^[0-9]+$'
	re='^[0-9]+(\.[0-9]+)?$'
	if ! [[ $2 =~ $re ]]
	then
		#echo "invalid second param: Not is a number"
		echo "invalid second param: Not is a decimal number"
		exit 0
	fi
	#bash doesn't support aritmetic operation with decimals , but bc yes
	if [ $(echo "$2>$MAXSTEP"|bc) -gt 0 ]
	then
		REALSTEP=$MAXSTEP
	elif [ $(echo "$2<$MINSTEP"|bc) -gt 0 ]
	then
		REALSTEP=$MINSTEP
	else
		REALSTEP=$2
	fi
	GCCARGS="-O3 -DTIME_ONE_STEP=$REALSTEP -std=c99"
	#GCCARGS="-O3  -D TIME_ONE_STEP=$REALSTEP -D_POSIX_C_SOURCE=199309L -lrt -std=c99"
	RUNFORESTRUN="mpirun -np 666"
	
elif [ $1 == "TEST" ]
then
	echo
	echo "compiling environment TEST"
	COMPILER="gcc"
	RUNFORESTRUN=" "
	if [ $2 == "SCHEDULER" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _MOCK_SCHADULER_"
	elif [ $2 == "PRINTER" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _MOCK_PRINTER_"
	elif [ $2 == "RAFFLER" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _MOCK_RAFFLER_"
	elif [ $2 == "GENERIC" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _MOCK_NODE_"
	elif [ $2 == "QUEUE" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _QUEUE_NODE_"
	elif [ $2 == "COMBI" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _COMBI_NODE_"
	elif [ $2 == "NORMAL" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _NORMAL_NODE_"
	elif [ $2 == "FUNCTION" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _FUNCTION_NODE_"
	elif [ $2 == "COUNTER" ]
	then
		GCCARGS="-std=c99 -D _MOCK_MPI_ -D _COUNTER_NODE_"
	else
		echo "invalid second param"
		echo
		exit 0
	fi
else
	echo
	echo "invalid first param"
	echo
	exit 0
fi

echo "$COMPILER -Wall $GCCARGS -c RNGs.c"
$COMPILER -Wall $GCCARGS -c RNGs.c

echo "$COMPILER -Wall $GCCARGS -c parson.c"
$COMPILER -Wall $GCCARGS -c parson.c

echo "$COMPILER -Wall $GCCARGS -c jsonHelper.c"
$COMPILER -Wall $GCCARGS -c jsonHelper.c

echo "$COMPILER -Wall $GCCARGS -c genericNode.c"
$COMPILER -Wall $GCCARGS -c genericNode.c

echo "$COMPILER -Wall $GCCARGS -c queue.c"
$COMPILER -Wall $GCCARGS -c queue.c

echo "$COMPILER -Wall $GCCARGS -c combi.c"
$COMPILER -Wall $GCCARGS -c combi.c

echo "$COMPILER -Wall $GCCARGS -c normal.c"
$COMPILER -Wall $GCCARGS -c normal.c

echo "$COMPILER -Wall $GCCARGS -c function.c"
$COMPILER -Wall $GCCARGS -c function.c

echo "$COMPILER -Wall $GCCARGS -c counter.c"
$COMPILER -Wall $GCCARGS -c counter.c

echo "$COMPILER -Wall $GCCARGS -c printer.c"
$COMPILER -Wall $GCCARGS -c printer.c

echo "$COMPILER -Wall $GCCARGS -c raffler.c"
$COMPILER -Wall $GCCARGS -c raffler.c

echo "$COMPILER -Wall $GCCARGS -c scheduler.c"
$COMPILER -Wall $GCCARGS -c scheduler.c

echo "$COMPILER -Wall $GCCARGS RNGs.o parson.o jsonHelper.o raffler.o printer.o genericNode.o queue.o combi.o normal.o function.o counter.o scheduler.o -lm  -lwjelement -lwjreader -o $BOTQUEUE_CODE main.c"
$COMPILER -Wall $GCCARGS RNGs.o parson.o jsonHelper.o raffler.o printer.o genericNode.o queue.o combi.o normal.o function.o counter.o scheduler.o -lm -lwjelement -lwjreader -o $BOTQUEUE_CODE main.c

rm *.o
