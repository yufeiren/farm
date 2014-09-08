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
script=$Taskdir/$mode-$bs-$iodepth
touch $script
echo "[global]" > $script
echo "ioengine=rdma" >> $script
echo "" >> $script
# task
job=[$mode-$bs-$iodepth]
echo $job >> $script
echo "thread" >> $script
echo "filename="$ServIP/$ServPort/$mode >> $script
echo "rw=write" >> $script
if [ $bs = "512" ] || [ $bs = "1k" ] || [ $bs = "2k" ]; then
	echo "size=20g" >> $script
else
	echo "size="$DataSize >> $script
fi
echo "bs="$bs >> $script
echo "iodepth="$iodepth >> $script

ServPort=$(($ServPort+10))

$Fio --minimal $script >> $LogFile

sleep 5
		done
	done
done

