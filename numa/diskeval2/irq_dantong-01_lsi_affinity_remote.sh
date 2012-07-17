#! /bin/sh

irqs="156 157"

for irq in $irqs
do
	echo "11110000" > /proc/irq/$irq/smp_affinity
done

service irqbalance stop
