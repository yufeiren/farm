#! /bin/sh

# dantong-2 eth10
irq=264

while [ $irq -le 279 ]; do
	echo "ffffffff" > /proc/irq/$irq/smp_affinity
	(( irq++ ))
done

service irqbalance stop
