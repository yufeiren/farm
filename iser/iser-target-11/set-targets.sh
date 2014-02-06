#! /bin/bash

tgtadm -C 1 --lld iser --op new --mode target --tid=1 --targetname Srv365-11.cewit.stonybrook.edu:iser.sdb

tgtadm -C 1 --lld iser --op show --mode target

tgtadm -C 1 --lld iser --op new --mode logicalunit --tid 1 --lun 1 -b /mnt/sdb/hg

tgtadm -C 1 --lld iser --op bind --mode target --tid 1 -I ALL

tgtadm -C 1 --lld iser --mode account --op new --user "consumer" --password "Longsw0rd"

tgtadm -C 1 --lld iser --mode account --op bind --tid 1 --user "consumer"

tgtadm -C 1 --lld iser --op show --mode target
