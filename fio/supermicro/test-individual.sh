#! /bin/bash

logfile=p46disk.log
echo > $logfile

for ie in sync libaio
do
	for rw in read randread write randwrite
	do
		fio $ie-$rw.fio >> $logfile
	done
done

