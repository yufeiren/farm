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

cat /dev/null > $Logdir/$TestSuite-thr.log
cat /dev/null > $Logdir/$TestSuite-lat.log

# throughput
run_test() {
for size in $sizes
do
	for rw in $rws
	do
		for bs in $bss
		do
# throughput eval
fio --minimal --rw=$rw --size=$size --ioengine=libaio --iodepth=8 --direct=1 --bs=$bs --name=w1 --filename=/dev/sdd --name=w2 --filename=/dev/sde --name=w3 --filename=/dev/sdf --name=w4 --filename=/dev/sdc >> $Logdir/$TestSuite-thr.log

# wait for write back finished
sleep 100
# latency eval
fio --minimal --rw=$rw --size=$size --ioengine=sync --iodepth=1 --direct=1 --bs=$bs --name=w1 --filename=/dev/sdd --name=w2 --filename=/dev/sde --name=w3 --filename=/dev/sdf --name=w4 --filename=/dev/sdc >> $Logdir/$TestSuite-lat.log
		done
	done
done
}

run_test

