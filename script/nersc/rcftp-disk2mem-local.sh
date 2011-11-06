#! /bin/bash

source /global/u2/s/shudong/sync/nersc/define.sh

hn=`hostname`

if [ $hn = "cvrani04" ] || [ $hn = "cvrani08" ] || [ $hn = "cvrani12" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc01 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani05" ] || [ $hn = "cvrani09" ] || [ $hn = "cvrani13" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc02 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani06" ] || [ $hn = "cvrani10" ] || [ $hn = "cvrani14" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc03 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani07" ] || [ $hn = "cvrani11" ] || [ $hn = "cvrani15" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc04 > $logdir/rcftp-local-$hn.log
fi
