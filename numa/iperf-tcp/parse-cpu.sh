#! /bin/sh

source ./defination

for start in 1 3 5 7 9 11
do
cmd="cat "$Logdir/"iperf-tcp-cpu.log | sed -n "
i=$start
while [ $i -lt 301 ]
do
        t="-e "
        t=$t$i
        t=$t"p "
        cmd=$cmd$t
        i=$(expr $i + 12)
done
cmd=$cmd" | cut -f 4 -d ' ' > "$Parsedir"/iperf-cpu-"$start".log"
echo $cmd
done
