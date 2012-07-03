#! /bin/bash

# evaluate iperf client

source ./defination

if [ ! -x $Iperf ]; then
        echo You need iperf installed to start io evaluation
        exit 1
fi

rm -rf $Logdir

test -d $Logdir || mkdir -p $Logdir
test -e $LogFile || touch $LogFile

for cpunode in $cpunodes
do
	for memnode in $memnodes
	do
		for bs in $bss
		do
			for ps in $pss
			do
numactl --cpunodebind=$cpunode --membind=$memnode time -a -o $CpuLogFile iperf -c $servip -i 2 -l $bs -P $ps > $Logdir/iperf-c$cpunode-m$memnode-$bs-$ps.log

sleep 3
			done
		done
	done
done

