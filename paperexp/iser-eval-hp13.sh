#! /bin/bash

# start this script on Srv365-09

# eval node0 sdd1 sde1 sdg1

RunIOTests=/home/ren/git/farm/fio/run_io_tests.sh
Logdir=~/ipdps13/

# unchanged
expet="read randread write randwrite"
bs="64 256 512 1024 4096 8192"
ts="1 2 4 8 16"
timeperiod=30

# sync 1 file over ext4
flist="/home/ren/iser/d0/f00"
engine="sync"
iodepths="1"

numactl --cpunodebind=0 --membind=0 $RunIOTests -x "$expet" -f "$flist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -o $Logdir/iser-sync-f1.log > $Logdir/script.log 2>&1

# libaio 1 file over ext4
flist="/home/ren/iser/d0/f00"
engine="libaio"
iodepths="1 4 16 32"

numactl --cpunodebind=0 --membind=0 $RunIOTests -x "$expet" -f "$flist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -o $Logdir/iser-libaio-f1.log > $Logdir/script.log 2>&1

# sync 6 files over ext4
flist="/home/ren/iser/d0/f00 /home/ren/iser/d0/f01 /home/ren/iser/d1/f00 /home/ren/iser/d1/f01 /home/ren/iser/d2/f00 /home/ren/iser/d2/f01"
engine="sync"
iodepths="1"

numactl --cpunodebind=0 --membind=0 $RunIOTests -x "$expet" -f "$flist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -o $Logdir/iser-sync-f6.log > $Logdir/script.log 2>&1

# libaio 6 file over ext4
flist="/home/ren/iser/d0/f00 /home/ren/iser/d0/f01 /home/ren/iser/d1/f00 /home/ren/iser/d1/f01 /home/ren/iser/d2/f00 /home/ren/iser/d2/f01"
engine="libaio"
iodepths="1 4 16 32"

numactl --cpunodebind=0 --membind=0 $RunIOTests -x "$expet" -f "$flist" -p -I -a fio -e "$engine" -b "$bs" -t "$ts" -i "$iodepths" -l $timeperiod -z 3 -o $Logdir/iser-libaio-f6.log > $Logdir/script.log 2>&1

