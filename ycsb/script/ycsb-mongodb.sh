#! /bin/bash

source ./defination

test -d $Logdir || mkdir -p $Logdir

for wl in $wls
do
	$YCSBHOME/bin/ycsb run mongodb -P $Taskdir/$wl -s -threads 10 > $Logdir/$wl.log
done

