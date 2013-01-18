#! /bin/bash

# evaluate the tgt performance of 4 nodes NUMA host: srv365-11
# test with zipf distribution

source ./defination

if [ ! -x $Fio ]; then
        echo You need fio installed to start io evaluation
        exit 1
fi

rm -rf $Logdir
rm -rf $Taskdir

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir
test -e $DateFile || touch $DateFile

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
					for zipf in $zipfs
					do
# iodepth is meaningless for sync engine
if [ $ioengine = "sync" ] && [ $iodepth -gt 1 ]; then
	continue;
fi

# generate jobfile
task1=c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$zipf-sdc
task2=c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$zipf-sdd

echo "[global]" > $Taskdir/$task1
echo "ioengine="$ioengine >> $Taskdir/$task1
echo "direct=1" >> $Taskdir/$task1
echo "rw="$rw >> $Taskdir/$task1
echo "bs="$bs >> $Taskdir/$task1
echo "time_based" >> $Taskdir/$task1
echo "runtime="$Runtime >> $Taskdir/$task1
echo "size="$DataSize >> $Taskdir/$task1
echo "iodepth="$iodepth >> $Taskdir/$task1
echo "numa_cpu_nodes=0" >> $Taskdir/$task1
echo "numa_mem_policy=bind:0" >> $Taskdir/$task1
echo "numjobs="$numjobs >> $Taskdir/$task1
echo "random_distribution=zipf:"$zipf >> $Taskdir/$task1
echo "" >> $Taskdir/$task1

echo "[/dev/sdc]" >> $Taskdir/$task1

echo "[/dev/sdd]" >> $Taskdir/$task1

echo "[global]" > $Taskdir/$task2
echo "ioengine="$ioengine >> $Taskdir/$task2
echo "direct=1" >> $Taskdir/$task2
echo "rw="$rw >> $Taskdir/$task2
echo "bs="$bs >> $Taskdir/$task2
echo "time_based" >> $Taskdir/$task2
echo "runtime="$Runtime >> $Taskdir/$task2
echo "size="$DataSize >> $Taskdir/$task2
echo "iodepth="$iodepth >> $Taskdir/$task2
echo "numa_cpu_nodes=0" >> $Taskdir/$task2
echo "numa_mem_policy=bind:0" >> $Taskdir/$task2
echo "numjobs="$numjobs >> $Taskdir/$task2
echo "random_distribution=zipf:"$zipf >> $Taskdir/$task2
echo "" >> $Taskdir/$task2

echo "[/dev/sdd]" >> $Taskdir/$task2

echo "[/dev/sde]" >> $Taskdir/$task2

datestart=`date "+%F %H:%M:%S"`

log=$Logdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$zipf.log
if [ $cli -eq 1 ]; then
	job=$Taskdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$zipf-sdc
	$Fio --client=srv365-09.cewit.stonybrook.edu $job > $log
elif [ $cli -eq 2 ]; then
	job=$Taskdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$zipf-sdc
	$Fio --client=srv365-09.cewit.stonybrook.edu $job --client=srv365-07.cewit.stonybrook.edu $job > $log
elif [ $cli -eq 4 ]; then
	job1=$Taskdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$zipf-sdc
	job2=$Taskdir/c$cli-$ioengine-$rw-$bs-$iodepth-$numjobs-$zipf-sdd
	$Fio --client=srv365-09.cewit.stonybrook.edu $job1 --client=srv365-07.cewit.stonybrook.edu $job1 --client=srv365-15.cewit.stonybrook.edu $job2 --client=srv365-13.cewit.stonybrook.edu $job2 > $log
fi

dateend=`date "+%F %H:%M:%S"`
echo $datestart","$dateend >> $DateFile

sleep 5

						done
					done
				done
			done
		done
	done
done

