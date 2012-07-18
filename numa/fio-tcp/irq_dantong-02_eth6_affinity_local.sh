#! /bin/sh

# dantong-2 eth6
# cat /sys/devices/system/node/node2/cpulist
# 1,5,9,13
irq=340

while [ $irq -le 355 ]; do
	echo "00001111" > /proc/irq/$irq/smp_affinity
	(( irq++ ))
done

service irqbalance stop
