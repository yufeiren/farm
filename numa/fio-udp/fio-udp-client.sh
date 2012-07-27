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
for bs in $bss
do
	for cpunode in $cpunodes
	do
		for memnode in $memnodes
		do

script=$Taskdir/fio-udp-client-$bs
touch $script
echo "[global]" > $script
echo "ioengine=net" >> $script
echo "protocol=udp" >> $script
echo "" >> $script
# task
job=[$bs]
echo $job >> $script
echo "thread" >> $script
echo "rw=write" >> $script
echo "size="$DataSize >> $script
echo "bs="$bs >> $script
echo "hostname="$ServIP >> $script
echo "port="$ServPort >> $script
echo "time_based" >> $script
echo "runtime="$Runtime >> $script
echo "numa_bind_nodes="$Clientlocalnode >> $script
echo "rate="$SendingRate >> $script

ServPort=$(($ServPort+10))

$Fio --minimal $script >> $LogFile

sleep 3
		done
	done
done

