#! /bin/bash

# evaluate the raw disk performance of LSI in srv365-09

source ./defination

if [ ! -x $Fio ]; then
        echo You need fio installed to start io evaluation
        exit 1
fi

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir
test -e $LogFile || touch $LogFile

for ioengine in $ioengines
do
	for rw in $rws
	do
		for iodepth in $iodepths
		do
			for bs in $bss
			do
# iodepth is meaningless for sync engine
#if [ $ioengine = "sync" ] && [ $iodepth -gt 1 ]; then
#	continue;
#fi

# for sync, iodepth means parallel jobs.
if [ $ioengine = "sync" ]; then
	$Fio --time_based --runtime=$Runtime --thread --direct=1 --minimal --filesize=$DataSize --ioengine=$ioengine --rw=$rw --bs=$bs --numjobs=$iodepth --group_reporting --randrepeat=0 --name=$Disk --numa_cpu_nodes=$Numanode --numa_mem_policy=bind:$Numanode >> $LogFile
else
	$Fio --time_based --runtime=$Runtime --thread --direct=1 --minimal --filesize=$DataSize --ioengine=$ioengine --rw=$rw --bs=$bs --iodepth=$iodepth --name=$Disk --numa_cpu_nodes=$Numanode --numa_mem_policy=bind:$Numanode >> $LogFile
fi

sleep 5
			done
		done
	done
done

