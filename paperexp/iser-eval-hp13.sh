#! /bin/bash

# start this script on Srv365-09

# eval node0 sdd1 sde1 sdg1

RunIOTests=/home/ren/git/farm/fio/run_io_tests.sh
Logdir=/home/ren/ipdps13/

if [ ! -d $Logdir ]; then
	mkdir -p $Logdir
fi

# unchanged
expet="read randread write randwrite"
bs="64 256 512 1024 4096 8192"
ts="1 2 4 8 16"
timeperiod=30

# sync 1 raw device
devlist="/dev/sdd"
engine="sync"
iodepths="1"

numactl --cpunodebind=0 --membind=0 $RunIOTests -x "$expet" -d "$devlist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -v -o $Logdir/iser-sync-f1.log > $Logdir/script.log 2>&1

# libaio 1 raw device
devlist="/dev/sdd"
engine="libaio"
iodepths="1 4 16 32"

numactl --cpunodebind=0 --membind=0 $RunIOTests -x "$expet" -d "$devlist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -v -o $Logdir/iser-libaio-f1.log > $Logdir/script.log 2>&1

# sync 6 files over ext4
devlist="/dev/sdd /dev/sde /dev/sdg /dev/sdc /dev/sdf /dev/sdh"
engine="sync"
iodepths="1"

$RunIOTests -x "$expet" -d "$devlist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -v -o $Logdir/iser-sync-f6.log > $Logdir/script.log 2>&1

# libaio 6 file over ext4
devlist="/dev/sdd /dev/sde /dev/sdg /dev/sdc /dev/sdf /dev/sdh"
engine="libaio"
iodepths="1 4 16 32"

$RunIOTests -x "$expet" -d "$devlist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -v -o $Logdir/iser-libaio-f6.log > $Logdir/script.log 2>&1

