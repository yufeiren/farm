#! /bin/bash

# version 2.0.6

logfile=/home/ren/fio/log/rdma-test.log

Tersedir=/home/ren/fio/terse
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

# BW(kb) - 48
cut -f 48 -d ";" $logfile > $Tersedir/terse-write-bandwidth.log

# runtime(ms) - 49
cut -f 49 -d ";" $logfile > $Tersedir/terse-write-runtime.log

# total latency: mean: 81 - microsecond
cut -f 81 -d ";" $logfile > $Tersedir/terse-write-totallatencymean.log


# READ - server side

# IO(kb) - 6
cut -f 6 -d ";" $logfile > $Tersedir/terse-read-totalio.log

# BW(kb) - 7
cut -f 7 -d ";" $logfile > $Tersedir/terse-read-bandwidth.log

# runtime(ms) - 9
cut -f 9 -d ";" $logfile > $Tersedir/terse-read-runtime.log

# total latency: mean: 40 - microsecond
cut -f 40 -d ";" $logfile > $Tersedir/terse-read-totallatencymean.log

