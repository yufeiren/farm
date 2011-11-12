#! /bin/bash

source ./define.sh

# grep "bytes sent" rcftp-local* | cut -f 5 -d ' '

for host in $hosts
do
	ssh $username@$host "/global/u2/s/shudong/sync/nersc/rcftp-disk2mem-local.sh" &
done
