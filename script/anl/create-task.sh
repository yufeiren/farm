#! /bin/bash

source ./define.sh

rm -rf $taskdir/*

for host in $hosteths
do
	for dir in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
	do
	echo "open $host 18139" > $taskdir/get-$host-bc$dir
	echo "user ftp ftp" >> $taskdir/get-$host-bc$dir
	echo "prompt" >> $taskdir/get-$host-bc$dir
	echo "lcd $sinkdir" >> $taskdir/get-$host-bc$dir
	echo "cd $nerscsourcedir" >> $taskdir/get-$host-bc$dir
	echo "get bell_curve$dir" >> $taskdir/get-$host-bc$dir
	echo "bye" >> $taskdir/get-$host-bc$dir
	done
done

