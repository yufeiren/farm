#! /bin/bash

Fio=/usr/local/bin/fio
LocalDir=/data/fio
LocalDiskFileIn=/data/fio/readhg
LocalDiskFileOut=/data/fio/writehg
FileoutSize=100G
Logdir=/home/ren/fiotest/fio/log
Taskdir=/home/ren/fiotest/fio/task

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir

# ioengine: sync libaio
# direct: 1 0
# bs: 512K 1M 5M
# rw: read randread write randwrite

# read file
for ioengine in libaio sync
do
	for direct in 0 1
	do
		for bs in 512K 1M 5M 10M
		do
			for rw in read randread write randwrite
			do
sleep 5;
test -f $LocalDiskFileOut && rm -rf $LocalDiskFileOut
sync; echo 3 > /proc/sys/vm/drop_caches
script=$ioengine-$direct-$bs-$rw
touch $script
echo "[global]" > $script
echo "ioengine=$ioengine" >> $script
echo "direct=$direct" >> $script
echo "bs=$bs" >> $script
echo "rw=$rw" >> $script
echo "directory=$LocalDir" >> $script
echo "ioscheduler=deadline" >> $script
echo "time_based" >> $script
echo "runtime=60" >> $script
echo "" >> $script
echo "[job]" >> $script
echo "filename=readhg" >> $script
echo "size=100g" >> $script
echo "iodepth=16" >> $script
$Fio $script --bandwidth-log $script
			done	
		done
	done
done
