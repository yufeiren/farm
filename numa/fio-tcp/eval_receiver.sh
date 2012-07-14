#! /bin/bash

source ./defination

script_home=/home/ren/git/farm/numa/fio-tcp

for ps in $pss
do
	for bs in $bss
	do

$script_home/irq_dantong-02_eth10_balance.sh
$script_home/fio-tcp-server.sh $bs $ps balance

$script_home/irq_dantong-02_eth10_nonbalance.sh
$script_home/fio-tcp-server.sh $bs $ps nonbalance

$script_home/irq_dantong-02_eth10_affinity_local.sh
$script_home/fio-tcp-server.sh $bs $ps affinity_local

$script_home/irq_dantong-02_eth10_affinity_remote.sh
$script_home/fio-tcp-server.sh $bs $ps affinity_remote
	done
done

