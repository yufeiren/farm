#! /bin/bash

source /global/u2/s/shudong/sync/nersc/define.sh

hn=`hostname`

env RCFTPRC=$configdir/rcftprc-directio-1M-16 $rcftpbin -n -i -v < $taskdir/put-local > $logdir/rcftp-local-$hn.log

