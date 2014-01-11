#! /bin/bash

mkdir -p /home/ren/rftp/data/source-tmpfs/d
mkdir -p /home/ren/rftp/data/source-tmpfs/d2
mkdir -p /home/ren/rftp/data/sink-tmpfs/d
mkdir -p /home/ren/rftp/data/sink-tmpfs/d2

ln -s /home/ren/iser/d0/f00 /home/ren/rftp/data/source-tmpfs/d/d0-f00
ln -s /home/ren/iser/d0/f01 /home/ren/rftp/data/source-tmpfs/d/d0-f01
ln -s /home/ren/iser/d1/f00 /home/ren/rftp/data/source-tmpfs/d/d1-f00
ln -s /home/ren/iser/d1/f01 /home/ren/rftp/data/source-tmpfs/d/d1-f01
ln -s /home/ren/iser/d2/f00 /home/ren/rftp/data/source-tmpfs/d/d2-f00
ln -s /home/ren/iser/d2/f01 /home/ren/rftp/data/source-tmpfs/d/d2-f01

ln -s /home/ren/iser/d3/f00 /home/ren/rftp/data/source-tmpfs/d2/d3-f00
ln -s /home/ren/iser/d3/f01 /home/ren/rftp/data/source-tmpfs/d2/d3-f01
ln -s /home/ren/iser/d4/f00 /home/ren/rftp/data/source-tmpfs/d2/d4-f00
ln -s /home/ren/iser/d4/f01 /home/ren/rftp/data/source-tmpfs/d2/d4-f01
ln -s /home/ren/iser/d5/f00 /home/ren/rftp/data/source-tmpfs/d2/d5-f00
ln -s /home/ren/iser/d5/f01 /home/ren/rftp/data/source-tmpfs/d2/d5-f01

ln -s /home/ren/iser/d0/f00 /home/ren/rftp/data/sink-tmpfs/d/d0-f00
ln -s /home/ren/iser/d0/f01 /home/ren/rftp/data/sink-tmpfs/d/d0-f01
ln -s /home/ren/iser/d1/f00 /home/ren/rftp/data/sink-tmpfs/d/d1-f00
ln -s /home/ren/iser/d1/f01 /home/ren/rftp/data/sink-tmpfs/d/d1-f01
ln -s /home/ren/iser/d2/f00 /home/ren/rftp/data/sink-tmpfs/d/d2-f00
ln -s /home/ren/iser/d2/f01 /home/ren/rftp/data/sink-tmpfs/d/d2-f01

ln -s /home/ren/iser/d3/f00 /home/ren/rftp/data/sink-tmpfs/d2/d3-f00
ln -s /home/ren/iser/d3/f01 /home/ren/rftp/data/sink-tmpfs/d2/d3-f01
ln -s /home/ren/iser/d4/f00 /home/ren/rftp/data/sink-tmpfs/d2/d4-f00
ln -s /home/ren/iser/d4/f01 /home/ren/rftp/data/sink-tmpfs/d2/d4-f01
ln -s /home/ren/iser/d5/f00 /home/ren/rftp/data/sink-tmpfs/d2/d5-f00
ln -s /home/ren/iser/d5/f01 /home/ren/rftp/data/sink-tmpfs/d2/d5-f01

