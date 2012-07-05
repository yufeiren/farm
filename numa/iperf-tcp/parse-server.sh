#! /bin/sh

source ./defination

test -e $Parsedir || mkdir -p $Parsedir

# bandwidth

for cpunode in $cpunodes
do
	for memnode in $memnodes
	do

# example output
# [  4]  0.0-60.0 sec  63.3 GBytes  9.06 Gbits/sec
tail -1 $Logdir/iperf-server-c$cpunode-m$memnode.log | cut -c 35-39 >> $Parsedir/iperf-server-bw.log

	done
done

