#!/bin/sh
while [ "1" = "1" ]
do
sleep 10
if ! ifconfig eth0 | grep "RUNNING" >& /dev/null
then
#echo "into"
route del default >& /dev/null
route add default gw 10.64.64.64 >& /dev/null

fi

done

