#! /bin/bash

source ./define.sh

# direct io
for c in $cbufsizs
do
        for s in $rcstreamnums
        do
		for dir in 1 2 3 4 5 6 7
		do
grep "bytes sent" $logdir/rcftp-directio-bc$dir-$c-d$s.log | cut -f 5 -d ' '
		done
	done
done

