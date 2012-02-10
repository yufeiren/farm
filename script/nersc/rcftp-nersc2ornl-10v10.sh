#! /bin/bash

source /global/u2/s/shudong/sync/nersc/define.sh

config=$1
task=$2

env RCFTPRC=$config $rcftpbin -n -i -v < $task > $logdir/rcftp-local-$hn.log

	done
done

hn=`hostname`

env RCFTPRC=$configdir/rcftprc-directio-1M-16 $rcftpbin -n -i -v < $taskdir/put-local > $logdir/rcftp-local-$hn.log


env RCFTPRC=/global/u2/s/shudong/rftp/config/rcftprc-directio-128K-32 /global/u2/s/shudong/rftp/rftp-latest-bin/rcftp 10.200.200.93 18139

lcd /global/u2/s/shudong/rftp/data/source/
cd /scratch/yren/rftp/sink/
put bell_curve1

