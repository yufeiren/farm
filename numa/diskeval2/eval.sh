#! /bin/bash

# ./fio-ssd.sh ioengine rw iodepth affinity_setup

source ./defination

script_home=/home/ren/git/farm/numa/diskeval2

for ioengine in $ioengines
do
	for rw in $rws
	do
		for iodepth in $iodepths
		do

# iodepth is meaningless for sync engine
if [ $ioengine = "sync" ] && [ $iodepth -gt 1 ]; then
	continue;
fi

#$script_home/irq_dantong-01_lsi_balance.sh
#$script_home/fio-ssd.sh $ioengine $rw $iodepth balance

#$script_home/irq_dantong-01_lsi_nonbalance.sh
#$script_home/fio-ssd.sh $ioengine $rw $iodepth nonbalance

$script_home/irq_dantong-01_lsi_affinity_local.sh
$script_home/fio-ssd.sh $ioengine $rw $iodepth affinity_local

#$script_home/irq_dantong-01_lsi_affinity_remote.sh
#$script_home/fio-ssd.sh $ioengine $rw $iodepth affinity_remote

			
		done
	done
done

