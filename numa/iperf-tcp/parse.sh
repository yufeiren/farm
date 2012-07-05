#! /bin/sh

source ./defination

test -e $Parsedir || mkdir -p $Parsedir

# bandwidth

for ps in $pss
do
	for bs in $bss
	do

for cpunode in $cpunodes
do
	for memnode in $memnodes
	do

# example output
# [  4]  0.0-60.0 sec  63.3 GBytes  9.06 Gbits/sec
tail -1 $Logdir/iperf-c$cpunode-m$memnode-$bs-$ps.log | cut -c 35-39 >> $Parsedir/iperf-$bs-$ps.log

	done
done

	done
done

# cpu

for start in 1 3 5 7 9 11
do
cmd="cat "$Logdir/"iperf-tcp-cpu.log | sed -n "
i=1
while [ $i -lt 301]
do
	t="-e "
	t=$t$i
	t=$t"p "
	cmd=$cmd$t
	i=$(expr $i + 12)
done
cmd=$cmd" | cut -f 4 -d ' '"
sh $cmd > $Parsedir/iperf-cpu-$start.log
done

