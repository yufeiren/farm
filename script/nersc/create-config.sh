#! /bin/bash

source ./define.sh

rm -rf $configdir/*

cbufsiz="16K 64K 128K 512K 1M 2M"
rcstreamnum="1 2 4 8 16 32"

# rftpd server
for c in $cbufsiz
do
	touch $configdir/rftpdrc-b$c-p18139-dno
echo "cbufsiz=" $c > $configdir/rftpdrc-b$c-p18139-dno
echo "srvcomport=18139" >> $configdir/rftpdrc-b$c-p18139-dno
echo "directio=no" >> $configdir/rftpdrc-b$c-p18139-dno
done

# direct io
for c in $cbufsiz
do
        for s in $rcstreamnum
        do
                touch $configdir/rcftprc-directio-$c-$s
echo "cbufsiz=" $c > $configdir/rcftprc-directio-$c-$s
echo "rcstreamnum=" $s >> $configdir/rcftprc-directio-$c-$s
echo "directio=yes" >> $configdir/rcftprc-directio-$c-$s
        done
done

# sendfile
for s in $rcstreamnum
do
                touch $configdir/rcftprc-sendfile-$s
echo "rcstreamnum=" $s > $configdir/rcftprc-sendfile-$s
echo "usesendfile=yes" >> $configdir/rcftprc-sendfile-$s
done

# splice
for s in $rcstreamnum
do
                touch $configdir/rcftprc-splice-$s
echo "rcstreamnum=" $s > $configdir/rcftprc-splice-$s
echo "usesplice=yes" >> $configdir/rcftprc-splice-$s
done
