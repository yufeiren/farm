#! /bin/bash

source ./define.sh

rm -rf $configdir/*

# rftpd server
for c in $cbufsizs
do
	touch $configdir/rftpdrc-b$c-p18139-dno
echo "cbufsiz=" $c > $configdir/rftpdrc-b$c-p18139-dno
echo "srvcomport=18139" >> $configdir/rftpdrc-b$c-p18139-dno
echo "directio=no" >> $configdir/rftpdrc-b$c-p18139-dno
done

# direct io
for c in $cbufsizs
do
        for s in $rcstreamnums
        do
                touch $configdir/rcftprc-directio-$c-$s
echo "cbufsiz=" $c > $configdir/rcftprc-directio-$c-$s
echo "rcstreamnum=" $s >> $configdir/rcftprc-directio-$c-$s
echo "directio=yes" >> $configdir/rcftprc-directio-$c-$s
                touch $configdir/rcftprc-nondirectio-$c-$s
echo "cbufsiz=" $c > $configdir/rcftprc-nondirectio-$c-$s
echo "rcstreamnum=" $s >> $configdir/rcftprc-nondirectio-$c-$s
echo "directio=no" >> $configdir/rcftprc-nondirectio-$c-$s
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
