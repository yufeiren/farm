#! /bin/bash

source /global/u2/s/shudong/sync/nersc/define.sh

hn=`hostname`

bufsiz=128K
conn=16

if [ $hn = "cvrani01" ] || [ $hn = "cvrani09" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc1 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani02" ] || [ $hn = "cvrani10" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc2 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani04" ] || [ $hn = "cvrani11" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc3 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani05" ] || [ $hn = "cvrani12" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc4 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani06" ] || [ $hn = "cvrani13" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc5 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani07" ] || [ $hn = "cvrani14" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc6 > $logdir/rcftp-local-$hn.log
elif [ $hn = "cvrani08" ] || [ $hn = "cvrani15" ]; then
env RCFTPRC=$configdir/rcftprc-directio-$bufsiz-$conn $rcftpbin -n -i -v < $taskdir/put-local-bc7 > $logdir/rcftp-local-$hn.log
fi
