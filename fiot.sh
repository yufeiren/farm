#! /bin/bash

Fiotool=/home/ren/git/farm/fiotest
LocalDiskFile=/data/fiot/hundred
LocalDiskDir=/data/fiot
Logdir=/home/ren/fiotest

# ./fiotest -r/-w -o/-s -f /data/100Gtestfile2 -m 5M -i 1 -t 100M

# read file
for bufsiz in 1K 8K 32K 64K 128K 512K 1M 8M 32M 64M 128M
do
	sleep 5;
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -f $LocalDiskFile -m $bufsiz -i 2 > $Logdir/RD_$bufsiz.txt 2>&1
done
