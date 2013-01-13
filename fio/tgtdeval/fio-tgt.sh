#! /bin/bash

# evaluate the tgt performance of 4 nodes NUMA host: srv365-11

source ./defination

if [ ! -x $Fio ]; then
        echo You need fio installed to start io evaluation
        exit 1
fi

rm -rf $Logdir
rm -rf $Taskdir

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir
test -e $LogFile || touch $LogFile

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

# generate jobfile
for disk in "sdc sdd"
do
	task=c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$disk

	echo "[global]" > $Taskdir/$task
	echo "ioengine="$ioengine >> $Taskdir/$task
	echo "direct=1" >> $Taskdir/$task
	echo "rw="$rw >> $Taskdir/$task
	echo "bs="$bs >> $Taskdir/$task
	echo "time_based" >> $Taskdir/$task
	echo "runtime="$Runtime >> $Taskdir/$task
	echo "size="$DataSize >> $Taskdir/$task
	echo "iodepth="$iodepth >> $Taskdir/$task
	echo "" >> $Taskdir/$task

	echo "[/dev/"$disk"]" >> $Taskdir/$task
	echo "numa_cpu_nodes=0" >> $Taskdir/$task
	echo "numa_mem_policy=bind:0" >> $Taskdir/$task
	echo "numjobs="$numjobs >> $Taskdir/$task
done

log=$Logdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs.log
if [ $cli -eq 1 ]; then
	job=$Taskdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-sdc
	$Fio --client=srv365-09.cewit.stonybrook.edu $job > $log
elif [ $cli -eq 2 ]; then
	job=$Taskdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-sdc
	$Fio --client=srv365-09.cewit.stonybrook.edu $job --client=srv365-07.cewit.stonybrook.edu $job > $log
elif [ $cli -eq 4 ]; then
	job1=$Taskdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-sdc
	job2=$Taskdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-sdd
	$Fio --client=srv365-09.cewit.stonybrook.edu $job1 --client=srv365-07.cewit.stonybrook.edu $job1 --client=srv365-15.cewit.stonybrook.edu $job2 --client=srv365-13.cewit.stonybrook.edu $job2 > $log
fi

sleep 2

					done
				done
			done
		done
	done
done
