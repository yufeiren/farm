#! /bin/sh

irqs="156 157"

service irqbalance stop

for irq in $irqs
do
	echo "ffffffff" > /proc/irq/$irq/smp_affinity
done
