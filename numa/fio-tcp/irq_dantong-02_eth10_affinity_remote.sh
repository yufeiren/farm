#! /bin/sh

# dantong-2 eth10 to node 1
irq=264

while [ $irq -le 279 ]; do
	echo "11110000" > /proc/irq/$irq/smp_affinity
	(( irq++ ))
done

service irqbalance stop
