#! /bin/bash

# start this script on Srv365-15

# eval node0 sdb

RunIOTests=/home/ren/git/farm/fio/run_io_tests.sh
Logdir=~/ipdps13/

if [ ! -d $Logdir ]; then
	mkdir -p $Logdir
fi

devlist="/dev/sdb"

# unchanged
expet="read randread write randwrite"
bs="64 256 512 1024 4096 8192"
ts="1 2 4 8 16"
timeperiod=60

# sync 1 file over ext4
engine="sync"
iodepths="1"

numactl --cpunodebind=0 --membind=0 $RunIOTests -x "$expet" -d "$devlist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -o $Logdir/local-sync-f1.log > $Logdir/script.log 2>&1

# libaio 1 file over ext4
engine="libaio"
iodepths="1 4 16 32"

numactl --cpunodebind=0 --membind=0 $RunIOTests -x "$expet" -d "$devlist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -o $Logdir/iser-libaio-f1.log > $Logdir/script.log 2>&1

