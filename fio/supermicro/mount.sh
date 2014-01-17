#! /bin/bash

mkdir -p /mnt/sft-r0
mkdir -p /mnt/sft-r1
mkdir -p /mnt/sft-r2
mkdir -p /mnt/sft-r3

chown ren:ren /mnt/sft-r0
chown ren:ren /mnt/sft-r1
chown ren:ren /mnt/sft-r2
chown ren:ren /mnt/sft-r3



mkfs.xfs /dev/md124
mkfs.xfs /dev/md125
mkfs.xfs /dev/md126
mkfs.xfs /dev/md127

mount -t xfs /dev/md124 /mnt/sft-r0
mount -t xfs /dev/md125 /mnt/sft-r1
mount -t xfs /dev/md126 /mnt/sft-r2
mount -t xfs /dev/md127 /mnt/sft-r3


touch /mnt/sft-r0/f0
touch /mnt/sft-r0/f1
touch /mnt/sft-r0/f2
touch /mnt/sft-r1/f0
touch /mnt/sft-r1/f1
touch /mnt/sft-r1/f2
touch /mnt/sft-r2/f0
touch /mnt/sft-r2/f1
touch /mnt/sft-r2/f2
touch /mnt/sft-r3/f0
touch /mnt/sft-r3/f1
touch /mnt/sft-r3/f2




