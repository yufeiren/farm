#! /bin/bash

source ./define.sh

for host in $hosts
do
	ssh $username@$host "cd nmonlog/log && $nmonbin -fT -s 5 -c 17280"
done

