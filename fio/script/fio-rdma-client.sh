#! /bin/bash

Fio=/usr/local/bin/fio
if [ ! -x $Fio ]; then
	echo You need fio installed to start io evaluation
	exit 1
fi

DataSize=100g
Logdir=/home/ren/fio/log
Taskdir=/home/ren/fio/task

LogFile=$Logdir/rdma-test.log

ServIP=192.168.3.5
ServPort=19001;

rm -rf $Logdir
rm -rf $Taskdir

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir
test -e $LogFile || touch $LogFile

# server
for mode in RDMA_WRITE RDMA_READ SEND
do
	for bs in 512 1k 4k 32k 128k 512k 1m 4m 8m 16m 32m
	do
		for iodepth in 1 2 4 8 16 32 64
		do
script=$Taskdir/$mode-$bs
touch $script
echo "[global]" > $script
echo "ioengine=rdma" >> $script
# task
job=[$mode-$bs-$iodepth]
echo $job >> $script
#echo "thread" >> $script
echo "filename="$ServIP/$ServPort/$mode >> $script
echo "rw=write" >> $script
echo "bs="$bs >> $script
echo "size="$DataSize >> $script
echo "iodepth="$iodepth >> $script
#echo "client"
ServPort=$(($ServPort+1))

$Fio --minimal $script >> $LogFile
		done
	done
done

