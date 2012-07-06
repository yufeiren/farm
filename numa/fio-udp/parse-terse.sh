#! /bin/bash

# version 2.0.8

logfile=/home/ren/numa/fio-udp/log/fio-udp-test.log

Tersedir=/home/ren/numa/fio-udp/terse
test -d $Tersedir || mkdir -p $Tersedir


# cpu user - 88
cut -f 88 -d ";" $logfile > $Tersedir/terse-cpuuser.log
# cpu sys - 89
cut -f 89 -d ";" $logfile > $Tersedir/terse-cpusys.log
# context switches - 90
cut -f 90 -d ";" $logfile > $Tersedir/terse-contextswitch.log

# WRITE client side

# IO(kb) - 47
cut -f 47 -d ";" $logfile > $Tersedir/terse-write-totalio.log

# runtime(ms) - 49
cut -f 49 -d ";" $logfile > $Tersedir/terse-write-runtime.log

# total latency: mean: 81 - microsecond
cut -f 81 -d ";" $logfile > $Tersedir/terse-write-totallatencymean.log

# BW(kb) mean - 86
cut -f 86 -d ";" $logfile > $Tersedir/terse-write-bandwidth.log


# READ - server side

# IO(kb) - 6
cut -f 6 -d ";" $logfile > $Tersedir/terse-read-totalio.log

# runtime(ms) - 9
cut -f 9 -d ";" $logfile > $Tersedir/terse-read-runtime.log

# total latency: mean: 40 - microsecond
cut -f 40 -d ";" $logfile > $Tersedir/terse-read-totallatencymean.log

# BW(kb) mean - 45
cut -f 45 -d ";" $logfile > $Tersedir/terse-read-bandwidth.log

