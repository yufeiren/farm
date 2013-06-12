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

for bs in $bss
do
	fio --minimal --direct=1 --bs=$bs --rw=randread --ioengine=sync --iodepth=1 --size=$DataSize --name=f0 --filename=/mnt/sdc/fio/hg8  --rate_iops=10 --runtime=60 --time_based --randrepeat=0 --numa_cpu_nodes=0 --numa_mem_policy=bind:0 --thread >> $LogFile
	sleep 5
done

