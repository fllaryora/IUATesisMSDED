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
cd $BOTQUEUE_HOME
./compile.sh PRODUCTION 666

if [ -e "/tmp/benchmark.csv" ]
then
   if [ -f "/tmp/benchmark.csv" ]
   then
      echo "rm /tmp/benchmark.csv"
      rm /tmp/benchmark.csv
   else
      echo "no hay Engine"
   fi
else
   echo "no hay Engine"
fi

cp /home/francisco/Tesis/repo/IUATesisMSDED/Pruebas/benchmark/k13/modelo.json $BOTQUEUE_INPUT_FILE
for i in {1..32}
do
   echo "1-$i"
   mpirun -np 13 ./Engine 
done
cp /tmp/benchmark.csv /tmp/benchmark13.csv
rm /tmp/benchmark.csv
cp /home/francisco/Tesis/repo/IUATesisMSDED/Pruebas/benchmark/k18/modelo.json $BOTQUEUE_INPUT_FILE
for i in {1..32}
do
   echo "2-$i"
   mpirun -np 18 ./Engine 
done 
cp /tmp/benchmark.csv /tmp/benchmark18.csv
rm /tmp/benchmark.csv
cp /home/francisco/Tesis/repo/IUATesisMSDED/Pruebas/benchmark/k23/modelo.json $BOTQUEUE_INPUT_FILE
for i in {1..32}
do
   echo "3-$i"
   mpirun -np 23 ./Engine 
done 
cp /tmp/benchmark.csv /tmp/benchmark23.csv

