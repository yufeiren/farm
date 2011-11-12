#! /bin/bash

source ./define.sh

for host in $hosts
do
	ssh $username@$host "killall -s 9 rftpd"
done

