#! /bin/bash

# evaluate iperf server in different numanode

source ./defination

if [ ! -x $Iperf ]; then
        echo You need iperf installed to start io evaluation
        exit 1
fi

# rm -rf $Logdir/*
# test -d $Logdir || mkdir -p $Logdir

for cpunode in $cpunodes
do
	for memnode in $memnodes
	do
		numactl --cpunodebind=0 --membind=0 iperf -c $servip -t $timeperiod -l 128k -p $servport > /dev/null 2>&1
		(( servport++ ))
	done
done

