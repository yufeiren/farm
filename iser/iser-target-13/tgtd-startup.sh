#! /bin/bash

# start tgtd process on specified numa node
numactl --cpunodebind=0 --membind=0 tgtd -C 1 --iscsi portal=*:3261 --iser port=3261
numactl --cpunodebind=1 --membind=1 tgtd -C 2 --iscsi portal=*:3262 --iser port=3262
#tgtd -C 1 --iscsi portal=*:3261 --iser port=3261
#tgtd -C 2 --iscsi portal=*:3262 --iser port=3262

tgtadm -C 1 --op update --mode sys --name State -v offline
tgtadm -C 2 --op update --mode sys --name State -v offline

tgt-admin -C 1 -e -c ./targets-node0-ib0.conf
tgt-admin -C 2 -e -c ./targets-node1-ib1.conf

tgtadm -C 1 --op update --mode sys --name State -v ready
tgtadm -C 2 --op update --mode sys --name State -v ready

# setup account and password
tgtadm -C 1 --lld iser --mode account --op new --user "consumer" --password "Longsw0rd"
tgtadm -C 2 --lld iser --mode account --op new --user "consumer" --password "Longsw0rd"

tgtadm -C 1 --lld iser --mode account --op bind --tid 1 --user "consumer"
tgtadm -C 1 --lld iser --mode account --op bind --tid 2 --user "consumer"
tgtadm -C 1 --lld iser --mode account --op bind --tid 3 --user "consumer"
tgtadm -C 2 --lld iser --mode account --op bind --tid 1 --user "consumer"
tgtadm -C 2 --lld iser --mode account --op bind --tid 2 --user "consumer"
tgtadm -C 2 --lld iser --mode account --op bind --tid 3 --user "consumer"

# show setup result
tgtadm -C 1 --lld iser --mode target --op show
tgtadm -C 2 --lld iser --mode target --op show

