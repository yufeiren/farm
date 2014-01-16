#! /bin/bash

numactl --cpunodebind=1 --membind=1 fio --bs=512k --rw=write --ioengine=sync --size=50g --name=w0 --filename=/mnt/tmp1/iser-node1-target0
numactl --cpunodebind=1 --membind=1 fio --bs=512k --rw=write --ioengine=sync --size=50g --name=w1 --filename=/mnt/tmp1/iser-node1-target1
numactl --cpunodebind=1 --membind=1 fio --bs=512k --rw=write --ioengine=sync --size=50g --name=w2 --filename=/mnt/tmp1/iser-node1-target2
