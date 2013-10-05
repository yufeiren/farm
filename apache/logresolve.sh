#! /bin/bash

alog=/var/log/apache2/yufeiren.com/access.log
rlog=/home/ren/yufeiren.com/access.log
tlog=$rlog.tmp

logresolve < $alog > $tlog
# exclude access from hpdtl.cewit.stonybrook.edu
tac $tlog | sed '/hpdtl.cewit.stonybrook.edu/d' > $rlog
rm -f $tlog

