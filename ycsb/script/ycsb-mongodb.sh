#! /bin/bash

source ./defination

test -d $Logdir || mkdir -p $Logdir

$YCSBHOME/bin/ycsb load mongodb -P $Taskdir/wlonet-read100-up0 -s -threads 10 > $Logdir/wlonet-read100-up0.log

for wl in $wls
do
	$YCSBHOME/bin/ycsb run mongodb -P $Taskdir/$wl -s -threads 10 > $Logdir/$wl.log
done

