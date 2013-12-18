#! /bin/bash

logfile=p4raid.log
echo > $logfile

for ie in sync libaio
do
	for rw in read randread write randwrite
	do
		fio raid-$ie-$rw.fio >> $logfile
	done
done

