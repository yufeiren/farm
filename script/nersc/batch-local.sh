#! /bin/bash

source ./define.sh

for host in $hosts
do
	ssh $username@$host "/global/u2/s/shudong/sync/nersc/rcftp-disk2mem-local.sh" &
done
