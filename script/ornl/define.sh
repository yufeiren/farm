#! /bin/bash

nmonbin=/global/u2/s/shudong/install/nmon/nmon

configdir=/home/dtyu/sc11/config
taskdir=/home/dtyu/sc11/task
logdir=/home/dtyu/sc11/log

sourcedir=/home/dtyu/sc11/data/source
nerscsourcedir=/global/u2/s/shudong/rftp/data/source
sinkdir=/home/dtyu/sc11/data/sink

binurl=http://hpdtl.cewit.stonybrook.edu/release/binary/v0.15-rc1.tar
scripturl=http://hpdtl.cewit.stonybrook.edu/meta/anl.tar

rftpbindir=/home/dtyu/sc11/bin
rftpdbin=$rftpbindir/rftpd
rcftpbin=$rftpbindir/rcftp

cbufsizs="16K 64K 128K 512K 1M 2M"
rcstreamnums="1 2 4 8 16 32"

username=dtyu

hosts="as0171 as0172 as0173 as0174 as0175 as0176 as0178 as0179 as0180 as0181 as0182 as0183 as0184 as0185"

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