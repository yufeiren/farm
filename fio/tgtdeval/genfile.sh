#! /bin/bash

size=50g
i=0
dir1=/mnt/sdb1
dir2=/mnt/sdb2

while [ $i -lt 18 ];
do
	file1=/mnt/$dir1/f$i
	file2=/mnt/$dir2/f$i
	touch $file
	fio --bs=512k --ioengine=libaio --iodepth=4 --rw=write --name=j0 --filename=$file1
	fio --bs=512k --ioengine=libaio --iodepth=4 --rw=write --name=j0 --filename=$file2
done

