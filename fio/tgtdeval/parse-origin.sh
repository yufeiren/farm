#! /bin/bash

# evaluate the tgt performance of 4 nodes NUMA host: srv365-11

source ./defination

Logdir=/home/ren/git/paper/tpds-numa-cache/log/fio-throughput/log-tgt-nc-t16-dual
Parsedir=/home/ren/fio/parse-tgt-nc-t16-all

test -d $Parsedir || mkdir -p $Parsedir

for cli in $nr_initiator
do
for ioengine in $ioengines
do
	for rw in $rws
	do
		for bs in $bss
		do
			for iodepth in $iodepths
			do
				for numjobs in $numjobss
				do
# iodepth is meaningless for sync engine
if [ $ioengine = "sync" ] && [ $iodepth -gt 1 ]; then
	continue;
fi

log=$Logdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs.log
touch $Parsedir/parse-$cli.txt
#cat $log | grep aggrb | cut -f 2 -d ',' | cut -f 2 -d '=' >> $Parsedir/parse-$cli.txt

#cat $log | grep -v "clat" | grep "lat (usec):" | cut -d ',' -f 3 | cut -d '=' -f 2 >> $Parsedir/parse-$cli-latency-us.txt
cat $log | grep "iops=" | cut -d ',' -f 3 | cut -d '=' -f 2 >> $Parsedir/parse-$cli-iops.txt
					done
				done
			done
		done
	done
done
