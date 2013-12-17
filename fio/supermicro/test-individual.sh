#! /bin/bash

for ie in sync libaio
do
	for rw in read randread write randwrite
	do
		fio $ie-$rw.fio > p46disk.log
	done
done

