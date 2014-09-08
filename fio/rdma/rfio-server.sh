#! /bin/bash

Fio=/home/ren/git/rfio/fio
if [ ! -x $Fio ]; then
	echo You need fio installed to start io evaluation
	exit 1
fi

DataSize=100g
Logdir=/home/ren/fiotest/rfio/log
Taskdir=/home/ren/fiotest/rfio/task

ServIP=192.168.1.14
ServPort=19001;

rm -rf $Logdir
rm -rf $Taskdir

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir

script=$Taskdir/rdma-server
touch $script
echo "[global]" > $script
echo "ioengine=rdma" >> $script
echo "" >> $script

# server
for mode in RDMA_WRITE RDMA_READ SEND
do
	for bs in 512 1k 4k 32k 128k 512k 1m 4m 8m 16m 32m
	do
# task
job=[$mode-$bs]
echo $job >> $script
echo "thread" >> $script
echo "filename="$ServIP/$ServPort >> $script
echo "rw=read" >> $script
echo "size="$DataSize >> $script
echo "bs="$bs >> $script
echo "iodepth=64" >> $script
echo "" >> $script
ServPort=$(($ServPort+1))
	done
done

$Fio --minimal $script
