stime1=$(($(date +%s%N)/1000000))
stime2=$(($(date +%s%N)/1000000))
stime3=$(($(date +%s%N)/1000000))
echo  $(( $stime3 - $stime1 ))