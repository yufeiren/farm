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

# client
for cpunode in $cpunodes
do
	for memnode in $memnodes
	do
script=$Taskdir/client-$bs-$ps-$bal
touch $script
echo "[global]" > $script
echo "ioengine=net" >> $script
echo "protocol=tcp" >> $script
echo "bs=128k" >> $script
echo "size="$DataSize >> $script
echo "thread" >> $script
echo "rw=write" >> $script
echo "" >> $script

# task
i=1
while [ $i -le $ps ]; do
	job=[$bs-$ps-$bal-$i]
	echo $job >> $script
	echo "hostname="$ServIP >> $script
	echo "port="$ServPort >> $script
	echo "numa_cpu_nodes=4" >> $script
	echo "numa_mem_nodes=4" >> $script
	echo "" >> $script
	ServPort=$(($ServPort+10))
	(( i++ ))
done

$Fio --minimal $script >> $Logdir/tcp-test-client-$bs-$ps-$bal.log 2>&1

sleep 3
		done
	done
done

