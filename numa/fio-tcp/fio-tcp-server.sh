#! /bin/bash

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

bs=$1
ps=$2
bal=$3

# server
for cpunode in $cpunodes
do
	for memnode in $memnodes
	do
script=$Taskdir/server-$bs-$ps-$bal
touch $script
echo "[global]" > $script
echo "ioengine=net" >> $script
echo "protocol=tcp" >> $script
echo "bs="$bs >> $script
echo "size="$DataSize >> $script
echo "thread" >> $script
echo "rw=read" >> $script
echo "" >> $script

# task
i=1
while [ $i -le $ps ]; do
	job=[$bs-$ps-$bal-$i]
	echo $job >> $script
	echo "listen" >> $script
	echo "port="$ServPort >> $script
	echo "numa_cpu_nodes="$cpunode >> $script
	echo "numa_mem_nodes="$memnode >> $script
	echo "" >> $script
	ServPort=$(($ServPort+10))
	(( i++ ))
done

$Fio --minimal $script >> $Logdir/tcp-test-server-$bs-$ps-$bal.log 2>&1
		done
	done
done

