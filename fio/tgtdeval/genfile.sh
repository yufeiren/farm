#! /bin/bash

fsize=50g
i=0
dir1=/mnt/sdb1
dir2=/mnt/sdb4

while [ $i -lt 18 ];
do
	file1=$dir1/f$i
	file2=$dir2/f$i
	touch $file1
	touch $file2
	fio --direct=1 --size=$fsize --bs=512k --ioengine=libaio --iodepth=4 --rw=write --name=j0 --filename=$file1
	fio --direct=1 --size=$fsize --bs=512k --ioengine=libaio --iodepth=4 --rw=write --name=j0 --filename=$file2
done

