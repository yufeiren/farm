#! /bin/sh

irqs="156 157"

for irq in $irqs
do
	echo "00004444" > /proc/irq/$irq/smp_affinity
done

service irqbalance stop
