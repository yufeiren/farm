#! /bin/bash

# evaluate iperf server in different numanode

source ./defination

if [ ! -x $Iperf ]; then
        echo You need iperf installed to start io evaluation
        exit 1
fi

# rm -rf $Logdir/*

test -d $Logdir || mkdir -p $Logdir

for cpunode in $cpunodes
do
	for memnode in $memnodes
	do
		numactl --cpunodebind=$cpunode --membind=$memnode $Iperf -s -p $servport > $Logdir/iperf-server-c$cpunode-m$memnode.log 2>&1 &
		(( servport++ ))
	done
done

