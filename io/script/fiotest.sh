#! /bin/bash

Fio=$(which fio)
if [ ! -x $Fio ]; then
	echo You need fio installed to start io evaluation
	exit 1
fi

LocalDir=/data/fio
Testfile=fio-read-bench.dat
FileoutSize=1000G
Logdir=/home/ren/fiotest/fio/log
Taskdir=/home/ren/fiotest/fio/task
Runtime=600

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir

# read/write file test
for rw in read write
do
	for direct in 0 1
	do
		for bs in 8K 64K 512K 1M 5M 10M
		do
			for ioengine in sync psync vsync libaio posixaio splice
			do
sleep 5;
sync; echo 3 > /proc/sys/vm/drop_caches
if [ $rw = "write" ]; then
	test -f $LocalDir/$Testfile && rm -rf $LocalDir/$Testfile
fi
script=$Taskdir/$rw-$direct-$bs-$ioengine
touch $script
echo "[global]" > $script
echo "ioengine=$ioengine" >> $script
echo "direct=$direct" >> $script
# splice bug in 2.6.18
if [ $ioengine = "splice" ]; then
	echo "bs=8K" >> $script
else
	echo "bs=$bs" >> $script
fi
echo "rw=$rw" >> $script
echo "directory=$LocalDir" >> $script
echo "ioscheduler=deadline" >> $script
echo "time_based" >> $script
echo "runtime=$Runtime" >> $script
echo "" >> $script
echo "[job]" >> $script
echo "filename=$Testfile" >> $script
# write needs file size
if [ $rw = "write" ]; then
        echo "size=$FileoutSize" >> $script
fi
echo "iodepth=16" >> $script
$Fio --minimal $script
			done
		done
	done
done
