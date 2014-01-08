#! /bin/bash

numactl --cpunodebind=0 --membind=0 fio --bs=512k --rw=write --ioengine=sync --size=50g --name=w0 --filename=/mnt/tmp0/iser-node0-target0
numactl --cpunodebind=0 --membind=0 fio --bs=512k --rw=write --ioengine=sync --size=50g --name=w1 --filename=/mnt/tmp0/iser-node0-target1
numactl --cpunodebind=0 --membind=0 fio --bs=512k --rw=write --ioengine=sync --size=50g --name=w2 --filename=/mnt/tmp0/iser-node0-target2

