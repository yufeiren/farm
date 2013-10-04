#! /bin/bash

alog=/var/log/apache2/yufeiren.com/access.log
rlog=/home/ren/yufeiren.com/access.log

logresolve < $alog > $rlog

