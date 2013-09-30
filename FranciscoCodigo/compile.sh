MINPARAMS=2
COMPILER=" "
GCCARGS=" "
RUNFORESTRUN=" "

if [ $# -lt "$MINPARAMS" ]
then
	echo
	echo "This script needs at least $MINPARAMS command-line arguments!"
	echo "first argument TEST or PRODUCTION"
	echo "second argument node type or all"
	echo
	exit 0
fi

if [ $1 == "PRODUCTION" ]
then
	echo
	echo "compiling environment PRODUCTION"
	COMPILER="mpicc"
	GCCARGS="-O3"
	export PATH=$PATH:/usr/lib64/mpich2/bin
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/mpich2/lib
	re='^[0-9]+$'
	if ! [[ $2 =~ $re ]]
	then
		echo "invalid second param: Not is a number"
		exit 0
	fi
	RUNFORESTRUN="mpirun -np$2"
	
elif [ $1 == "TEST" ]
then
	echo
	echo "compiling environment TEST"
	COMPILER="gcc"
	RUNFORESTRUN=" "
	if [ $2 == "SCHEDULER" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _MOCK_SCHADULER_"
	elif [ $2 == "PRINTER" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _MOCK_PRINTER_"
	elif [ $2 == "RAFFLER" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _MOCK_RAFFLER_"
	elif [ $2 == "GENERIC" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _MOCK_NODE_"
	elif [ $2 == "QUEUE" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _QUEUE_NODE_"
	elif [ $2 == "COMBI" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _COMBI_NODE_"
	elif [ $2 == "NORMAL" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _NORMAL_NODE_"
	elif [ $2 == "FUNCTION" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _FUNCTION_NODE_"
	elif [ $2 == "COUNTER" ]
	then
		GCCARGS="-D _MOCK_MPI_ -D _COUNTER_NODE_"
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

$COMPILER -Wall $GCCARGS -c genericNode.c

$COMPILER -Wall $GCCARGS -c printer.c

$COMPILER -Wall $GCCARGS -c raffler.c

$COMPILER -Wall $GCCARGS -c scheduler.c

$COMPILER -Wall $GCCARGS raffler.o printer.o genericNode.o scheduler.o -lm -o Engine main.c

$RUNFORESTRUN ./Engine
