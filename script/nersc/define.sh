#! /bin/bash

nmonbin=/global/u2/s/shudong/install/nmon/nmon

configdir=/global/u2/s/shudong/rftp/config
taskdir=/global/u2/s/shudong/rftp/task
logdir=/global/u2/s/shudong/rftp/log

sourcedir=/global/u2/s/shudong/rftp/data/source
sinkdir=/global/u2/s/shudong/rftp/data/sink

rftpdbin=/global/u2/s/shudong/rftp/rftp-latest-bin/rftpd
rcftpbin=/global/u2/s/shudong/rftp/rftp-latest-bin/rcftp

cbufsizs="16K 64K 128K 512K 1M 2M"
rcstreamnums="1 2 4 8 16 32"

username=shudong

cvrani01=128.55.56.224
cvrani02=128.55.56.225
cvrani03=128.55.56.226
cvrani04=128.55.56.227
cvrani05=128.55.56.228
cvrani06=128.55.56.229
cvrani07=128.55.56.230
cvrani08=128.55.56.231
cvrani09=128.55.56.232
cvrani10=128.55.56.233
cvrani11=128.55.56.234
cvrani12=128.55.56.235
cvrani13=128.55.56.236
cvrani14=128.55.56.237
cvrani15=128.55.56.238
cvrani16=128.55.56.239

hosts="$cvrani01 $cvrani02 $cvrani04 $cvrani05 $cvrani06 $cvrani07 $cvrani08 $cvrani09 $cvrani10 $cvrani11 $cvrani12 $cvrani13 $cvrani14 $cvrani15 $cvrani16"

