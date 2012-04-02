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

script=$Taskdir/rdma-server
touch $script
echo "[global]" > $script
echo "ioengine=rdma" >> $script
echo "" >> $script

# server
for mode in RDMA_WRITE RDMA_READ SEND
do
	for bs in $bss
	do
		for iodepth in $iodepths
		do
# task
job=[$mode-$bs-$iodepth]
echo $job >> $script
#echo "thread" >> $script
echo "filename="$ServIP/$ServPort >> $script
echo "rw=read" >> $script
if [ $bs = "512" || $bs = "1k" || $bs = "2k" ]; then
	echo "size=20g" >> $script
else
	echo "size="$DataSize >> $script
fi
echo "bs="$bs >> $script
echo "iodepth=1" >> $script
echo "" >> $script
ServPort=$(($ServPort+1))
		done
	done
done

$Fio --minimal $script

