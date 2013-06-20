#!/bin/bash

#test die statement bash
if [ ! -f /etc/solr.conf ]; then
	echo  "Missing config file /etc/solr.conf"
	exit 6
fi

#el comando daemon de linux ejecuta en backgound
#switch segun orden clasica para los servicios
case $1 in
	start)
		echo -n "Starting ${NAME}: "
		ls -l /etc/
		;;
	stop)
		action "Stopping ${NAME}: "	ls -l /usr/
		;;
	restart)
		$0 stop
		$0 start
		;;
	status)
		ls -l 
		;;
	*)
		echo "Usage: $0 {start|stop|restart|status}"
		exit 3
		;;
esac
