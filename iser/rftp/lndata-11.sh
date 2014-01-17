#! /bin/bash

rm -rf /home/ren/rftp/data/source-tmpfs/d/*
rm -rf /home/ren/rftp/data/source-tmpfs/d2/*
rm -rf /home/ren/rftp/data/sink-tmpfs/d/*
rm -rf /home/ren/rftp/data/sink-tmpfs/d2/*

mkdir -p /home/ren/rftp/data/source-tmpfs/d
mkdir -p /home/ren/rftp/data/source-tmpfs/d2
mkdir -p /home/ren/rftp/data/sink-tmpfs/d
mkdir -p /home/ren/rftp/data/sink-tmpfs/d2

ln -s /mnt/sft-r0/f0 /home/ren/rftp/data/source-tmpfs/d/d0-f00
ln -s /mnt/sft-r0/f1 /home/ren/rftp/data/source-tmpfs/d/d0-f01
ln -s /mnt/sft-r0/f2 /home/ren/rftp/data/source-tmpfs/d/d1-f00
ln -s /mnt/sft-r1/f0 /home/ren/rftp/data/source-tmpfs/d/d1-f01
ln -s /mnt/sft-r1/f1 /home/ren/rftp/data/source-tmpfs/d/d2-f00
ln -s /mnt/sft-r1/f2 /home/ren/rftp/data/source-tmpfs/d/d2-f01

ln -s /mnt/sft-r2/f0 /home/ren/rftp/data/source-tmpfs/d2/d3-f00
ln -s /mnt/sft-r2/f1 /home/ren/rftp/data/source-tmpfs/d2/d3-f01
ln -s /mnt/sft-r2/f2 /home/ren/rftp/data/source-tmpfs/d2/d4-f00
ln -s /mnt/sft-r3/f0 /home/ren/rftp/data/source-tmpfs/d2/d4-f01
ln -s /mnt/sft-r3/f1 /home/ren/rftp/data/source-tmpfs/d2/d5-f00
ln -s /mnt/sft-r3/f2 /home/ren/rftp/data/source-tmpfs/d2/d5-f01

ln -s /mnt/sft-r0/f0 /home/ren/rftp/data/sink-tmpfs/d/d0-f00
ln -s /mnt/sft-r0/f1 /home/ren/rftp/data/sink-tmpfs/d/d0-f01
ln -s /mnt/sft-r0/f2 /home/ren/rftp/data/sink-tmpfs/d/d1-f00
ln -s /mnt/sft-r1/f0 /home/ren/rftp/data/sink-tmpfs/d/d1-f01
ln -s /mnt/sft-r1/f1 /home/ren/rftp/data/sink-tmpfs/d/d2-f00
ln -s /mnt/sft-r1/f2 /home/ren/rftp/data/sink-tmpfs/d/d2-f01

ln -s /mnt/sft-r2/f0 /home/ren/rftp/data/sink-tmpfs/d2/d3-f00
ln -s /mnt/sft-r2/f1 /home/ren/rftp/data/sink-tmpfs/d2/d3-f01
ln -s /mnt/sft-r2/f2 /home/ren/rftp/data/sink-tmpfs/d2/d4-f00
ln -s /mnt/sft-r3/f0 /home/ren/rftp/data/sink-tmpfs/d2/d4-f01
ln -s /mnt/sft-r3/f1 /home/ren/rftp/data/sink-tmpfs/d2/d5-f00
ln -s /mnt/sft-r3/f2 /home/ren/rftp/data/sink-tmpfs/d2/d5-f01

