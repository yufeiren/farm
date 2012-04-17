#! /bin/bash

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

# server
for mode in RDMA_WRITE RDMA_READ SEND
do
	for iodepth in $iodepths
	do
		for bs in $bss
		do
script=$Taskdir/server-$mode-$bs-$iodepth
touch $script
echo "[global]" > $script
echo "ioengine=rdma" >> $script
echo "" >> $script

# task
job=[$mode-$bs-$iodepth]
echo $job >> $script
echo "thread" >> $script
echo "filename="$ServIP/$ServPort >> $script
echo "rw=read" >> $script
if [ $bs = "512" ] || [ $bs = "1k" ] || [ $bs = "2k" ]; then
	echo "size=20g" >> $script
else
	echo "size="$DataSize >> $script
fi
echo "bs="$bs >> $script
if [ $mode = "SEND" ]; then
	if [ $bs = "8m" ] || [ $bs = "16m" ]; then
		echo "iodepth=64" >> $script
	else
		echo "iodepth=256" >> $script
	fi
else
	echo "iodepth="$iodepth >> $script
fi
echo "" >> $script
ServPort=$(($ServPort+10))

$Fio --minimal $script >> $Logdir/rdma-test-server.log 2>&1
		done
	done
done


