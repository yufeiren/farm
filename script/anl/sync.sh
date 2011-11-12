#! /bin/bash

source ./define.sh

# env RCFTPRC=/home/dtyu/sc11/config/rcftprc-nondirectio-128K-16 /home/dtyu/sc11/bin/rcftp -n -i -v < /home/dtyu/sc11/task/get-10.200.200.1-bc1

# init
for host in $hosts
do
# mkdir
	ssh $username@$host "mkdir -p $configdir && mkdir -p $taskdir && mkdir -p $logdir && mkdir -p $sourcedir && mkdir -p $sinkdir && mkdir -p $rftpbindir"
	ssh $username@$host "mkdir -p install && cd install && rm -rf v0.15-rc1 && wget $binurl"

# update script and binary
	ssh $username@$host "cd install && tar xvf v0.15-rc1.tar && mv rftp-v0.15-rc1-bin/* $rftpbindir"
	ssh $username@$host "cd /home/dtyu/sc11/ && rm -rf anl.tar && wget $scripturl && tar xvf anl.tar"

# create config and task
	ssh $username@$host "cd /home/dtyu/sc11/anl && ./create-config.sh"
	ssh $username@$host "cd /home/dtyu/sc11/anl && ./create-task.sh"

# create data link
	ssh $username@$host "cd /home/dtyu/sc11/anl && ./lnbell.sh"
done

