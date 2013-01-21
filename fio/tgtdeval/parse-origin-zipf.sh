#! /bin/bash

# evaluate the tgt performance of 4 nodes NUMA host: srv365-11

source ./defination

test -d $Parsedir || mkdir -p $Parsedir

for cli in $nr_initiator
do
for ioengine in $ioengines
do
	for rw in $rws
	do
	for zipf in $zipfs
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

log=$Logdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$zipf.log
touch $Parsedir/parse-$cli.txt
cat $log | grep aggrb | cut -f 2 -d ',' | cut -f 2 -d '=' >> $Parsedir/parse-$cli.txt

						done
					done
				done
			done
		done
	done
done
