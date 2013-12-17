#! /bin/bash

# change disk io scheduler
# root previledge

# noop anticipatory deadline cfq
sched=noop

for disk in b c d e f g h i j k l m n o p q r s t u v w x y z
do
	echo $sched > /sys/block/sd$disk/queue/scheduler
	cat /sys/block/sd$disk/queue/scheduler
done

for disk in b c d e f g h i j k l m n o p q r s t u v
do
	echo $sched > /sys/block/sda$disk/queue/scheduler
	cat /sys/block/sda$disk/queue/scheduler
done

