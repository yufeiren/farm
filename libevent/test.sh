#! /bin/bash

export LD_LIBRARY_PATH=/usr/local/lib

Bench=/home/ren/git/farm/libevent/bench
Logdir=./

Pipes="100 1000 10000 100000"
Backend="select poll epoll"
NUMAsetup="numactl --cpunodebind 0 --membind 0"

for b in $Backend
do
	for n in $Pipes
	do
		$NUMAsetup $Bench -b $b -n $n > $Logdir/$b-$n.log 2>&1
	done
done

