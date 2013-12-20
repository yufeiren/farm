#! /bin/bash

# evaluate performance of a single disk

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

task=$Taskdir/$ioengine-$rw-$iodepth-$bs.fio

echo > $task
echo "[global]" >> $task
echo "time_based" >> $task
echo "runtime="$Runtime >> $task
echo "thread" >> $task
echo "direct=1" >> $task
echo "filesize="$DataSize >> $task
echo "ioengine="$ioengine >> $task
echo "rw="$rw >> $task
echo "bs="$bs >> $task
echo "group_reporting" >> $task
echo "randrepeat=0" >> $task
# for sync, iodepth means parallel jobs.
if [ $ioengine = "sync" ]; then
	echo "numjobs="$iodepth >> $task
else
	echo "iodepth="$iodepth >> $task
fi

i=0
while [ $i -lt ${#RAIDarr[*]} ]
do
	echo "["${RAIDarr[$i]}"]" >> $task
	echo "numa_cpu_nodes="${NUMAnodearr[$i]} >> $task
	echo "numa_mem_policy=bind:"${NUMAnodearr[$i]} >> $task
	(( i++ ))
done

#$Fio --minimal $task
#sleep 3
			done
		done
	done
done

