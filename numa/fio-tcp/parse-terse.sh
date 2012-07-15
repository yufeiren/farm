#! /bin/bash

# version 2.0.8

source ./defination

parse_pss="1"

modes="balance nonbalance affinity_local affinity_remote"

Tersedir=$Logdir/terse
test -d $Logdir/terse || mkdir -p $Logdir/terse

for ps in $parse_pss
do
	for mode in $modes
	do
		for bs in $bss
		do

logfile=$Logdir/tcp-test-server-$bs-$ps-$mode.log

# get even line of the log file
newlogfile=$Logdir/terse/tcp-test-server-$bs-$ps-$mode.log
awk '{if(NR%2==0)print}' $logfile > $newlogfile

# cpu user - 88
cut -f 88 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-cpuuser.log
# cpu sys - 89
cut -f 89 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-cpusys.log
# context switches - 90
cut -f 90 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-contextswitch.log

# WRITE client side

# IO(kb) - 47
cut -f 47 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-write-totalio.log

# runtime(ms) - 49
cut -f 49 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-write-runtime.log

# total latency: mean: 81 - microsecond
cut -f 81 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-write-totallatencymean.log

# BW(kb) mean - 86
cut -f 86 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-write-bandwidth.log


# READ - server side

# IO(kb) - 6
cut -f 6 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-read-totalio.log

# runtime(ms) - 9
cut -f 9 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-read-runtime.log

# total latency: mean: 40 - microsecond
cut -f 40 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-read-totallatencymean.log

# BW(kb) mean - 45
cut -f 45 -d ";" $newlogfile > $Tersedir/terse-$bs-$ps-$mode-read-bandwidth.log

		done
	done
done
