#! /bin/bash

source ./define.sh

for host in $hosts
do
	scp $username@$host:$logdir/* $logdir/
	ssh $username@$host "rm -rf $logdir/*"
done

# for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14; do grep "bytes received" rcftp-directio-bc$i-128K-d16.log | cut -f 5 -d ' '; done