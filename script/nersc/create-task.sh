#! /bin/bash

source ./define.sh

rm -rf $taskdir/*

# local
for dir in 1 2 3 4 5 6 7
do
	echo "open 127.0.0.1 18139" > $taskdir/put-local-bc$dir
	echo "user ftp ftp" >> $taskdir/put-local-bc$dir
	echo "prompt" >> $taskdir/put-local-bc$dir
	echo "lcd " $sourcedir >> $taskdir/put-local-bc$dir
	echo "cd " $sinkdir >> $taskdir/put-local-bc$dir
	echo "put bell_curve" $dir >> $taskdir/put-local-bc$dir
	echo "bye" >> $taskdir/put-local-bc$dir
done

for host in $hosts
do
	for dir in 1 2 3 4 5 6 7
	do
	echo "open " $host " 18139" > $taskdir/put-$host-bc$dir
	echo "user ftp ftp" >> $taskdir/put-$host-bc$dir
	echo "prompt" >> $taskdir/put-$host-bc$dir
	echo "lcd " $sourcedir >> $taskdir/put-$host-bc$dir
	echo "cd " $sinkdir >> $taskdir/put-$host-bc$dir
	echo "put bell_curve" $dir >> $taskdir/put-$host-bc$dir
	echo "bye" >> $taskdir/put-$host-bc$dir
	done
done

