#! /bin/bash

# fio

#clear
echo > dl.txt

# node 0
for disk in b c d e f g h i j k l m n o p q r s t u v w x
do
	echo "[/dev/sd$disk]" >> dl.txt
	echo "numa_cpu_nodes=0" >> dl.txt
	echo "numa_mem_policy=bind:0" >> dl.txt
	echo "" >> dl.txt
done

# node 1
for disk in y z
do
	echo "[/dev/sd$disk]" >> dl.txt
	echo "numa_cpu_nodes=1" >> dl.txt
	echo "numa_mem_policy=bind:1" >> dl.txt
	echo "" >> dl.txt
done

for disk in b c d e f g h i j k l m n o p q r s t u v
do
	echo "[/dev/sda$disk]" >> dl.txt
	echo "numa_cpu_nodes=1" >> dl.txt
	echo "numa_mem_policy=bind:1" >> dl.txt
	echo "" >> dl.txt
done

for ioengine in sync libaio
do
	for rw in read randread write randwrite
	do
		echo > $ioengine-$rw.fio
		cat global >> $ioengine-$rw.fio
		echo "ioengine=$ioengine" >> $ioengine-$rw.fio
		echo "rw=$rw" >> $ioengine-$rw.fio
		echo "" >> $ioengine-$rw.fio
		cat dl.txt >> $ioengine-$rw.fio
	done
done

