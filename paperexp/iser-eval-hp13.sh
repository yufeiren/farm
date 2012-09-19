#! /bin/bash

# start this script on Srv365-09

# eval node0 sdd1 sde1 sdg1

tc=auto

RunIOTests=/home/ren/git/farm/fio/run_io_tests.sh
Logdir=/home/ren/ipdps13/$tc

if [ ! -d $Logdir ]; then
	mkdir -p $Logdir
fi

# unchanged
expet="read randread write randwrite"
bs="64 256 512 1024 4096 8192"
ts="1 2 4 8 16"
timeperiod=300

# sync 6 files over ext4
devlist="/dev/sdd /dev/sde /dev/sdg /dev/sdc /dev/sdf /dev/sdh"
engine="sync"
iodepths="1"

# clean
echo "" > $Logdir/$tc-date-start.log
echo "" > $Logdir/$tc-date-end.log
echo "" > $Logdir/$tc-sum.log

for exp in $expet
do
	for b in $bs
	do
		for t in $ts
		do

date "+%F %H:%M:%S" >> $Logdir/$tc-date-start.log

$RunIOTests -x "$exp" -d "$devlist" -p -I -a fio -e "$engine" -b "$b" -t "$t" -i "$iodepths" -l $timeperiod -z 3 -v -o $Logdir/iser-sync-f6-$exp-b$b-t$t.log > $Logdir/script.log 2>&1

tail -1 $Logdir/iser-sync-f6-$exp-b$b-t$t.log >> $Logdir/$tc-sum.log

date "+%F %H:%M:%S" >> $Logdir/$tc-date-end.log

		done
	done
done

