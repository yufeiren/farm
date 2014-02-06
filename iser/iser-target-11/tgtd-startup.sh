#! /bin/bash

# start tgtd process on specified numa node
# /home/ren/sync/tgt/usr/tgtd -C 1 -t 16 -s 240g -c 512k -w 12 --iscsi portal=*:13261 --iser port=13261,pool_sz_mb=256

tgtadm -C 1 --op update --mode sys --name State -v offline

tgt-admin -C 1 -e -c ./targets.conf

tgtadm -C 1 --op update --mode sys --name State -v ready

# setup account and password
tgtadm -C 1 --lld iser --mode account --op new --user "consumer" --password "Longsw0rd"

tgtadm -C 1 --lld iser --mode account --op bind --tid 1 --user "consumer"
tgtadm -C 1 --lld iser --mode account --op bind --tid 2 --user "consumer"

# show setup result
tgtadm -C 1 --lld iser --mode target --op show

