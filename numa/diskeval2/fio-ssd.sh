#! /bin/bash

# evaluate the raw disk performance of LSI in HP/AMD

source ./defination

if [ ! -x $Fio ]; then
        echo You need fio installed to start io evaluation
        exit 1
fi

# rm -rf $Logdir
# rm -rf $Taskdir

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir
test -e $LogFile || touch $LogFile

ioengine=$1
rw=$2
iodepth=$3
bal=$4

suitelog=$Logdir/$TestSuite-$ioengine-$iodepth-$rw.log

for cpunode in $cpunodes
do
	for memnode in $memnodes
	do
		for bs in $bss
		do

$Fio --direct=1 --minimal --filesize=$DataSize --ioengine=$ioengine --rw=$rw --bs=$bs --iodepth=$iodepth --thread --time_based --runtime=$runtime --name=/dev/sdc --numa_cpu_nodes=$cpunode --numa_mem_nodes=$memnode >> $suitelog

sleep 3
		done
	done
done

