#! /bin/bash

source /global/u2/s/shudong/sync/nersc/define.sh

config=$1
task=$2

env RCFTPRC=$config $rcftpbin -n -i -v < $task > $logdir/rcftp-local-$hn.log

	done
done

hn=`hostname`

env RCFTPRC=$configdir/rcftprc-directio-1M-16 $rcftpbin -n -i -v < $taskdir/put-local > $logdir/rcftp-local-$hn.log

