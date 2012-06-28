#! /bin/bash

logdir=/home/ren/numa/numademo

mss="512m 1g 2g 4g"
ts="memset memcpy stream"
iters=5

i=0
while [ $i -lt $iters ]
do
	for ms in $mss
	do
		for t in $ts
		do
			touch $logdir/iter$i-$t-$ms.log
			numademo -t -c $ms $t > $logdir/iter$i-$t-$ms.log
		done
	done
	(( i++ ))
done

