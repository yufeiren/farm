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

#hosts="$cvrani01 $cvrani02 $cvrani04 $cvrani05 $cvrani06 $cvrani07 $cvrani08 $cvrani09 $cvrani10 $cvrani11 $cvrani12 $cvrani13 $cvrani14 $cvrani15 $cvrani16"
hosts="$cvrani01 $cvrani02 $cvrani04 $cvrani05 $cvrani06 $cvrani07 $cvrani08"

cvranieth01=10.200.200.1
cvranieth02=10.200.200.2
cvranieth03=10.200.200.3
cvranieth04=10.200.200.4
cvranieth05=10.200.200.5
cvranieth06=10.200.200.6
cvranieth07=10.200.200.7
cvranieth08=10.200.200.8
cvranieth09=10.200.200.9
cvranieth10=10.200.200.10
cvranieth11=10.200.200.11
cvranieth12=10.200.200.12
cvranieth13=10.200.200.13
cvranieth14=10.200.200.14
cvranieth15=10.200.200.15
cvranieth16=10.200.200.16

hosteths="$cvranieth01 $cvranieth02 $cvranieth04 $cvranieth05 $cvranieth06 $cvranieth07 $cvranieth08 $cvranieth09 $cvranieth10 $cvranieth11 $cvranieth12 $cvranieth13 $cvranieth14 $cvranieth15 $cvranieth16"