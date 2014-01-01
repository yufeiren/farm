#! /bin/bash

# result is in ganglia
# memdisk to /dev/null

ms="1 2 4 8 16 32 64"
cipher=aes128-cbc
file=/mnt/tmp0/f60g
period=600
dest=192.168.150.9

for m in $ms
do
	i=0
	while [ $i -lt $m ]
	do
		scp -c $cipher $file ren@$dest:/dev/null &
		sleep 1
		(( i++ ))
	done

	sleep $period
	killall -s 9 scp
	sleep 10
done

