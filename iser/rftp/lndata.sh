#! /bin/bash

mkdir -p /home/ren/rftp/data/source-tmpfs/d
mkdir -p /home/ren/rftp/data/sink-tmpfs/d

ln -s /home/ren/iser/d0/f00 /home/ren/rftp/data/source-tmpfs/d/d0-f00
ln -s /home/ren/iser/d0/f01 /home/ren/rftp/data/source-tmpfs/d/d0-f01
ln -s /home/ren/iser/d1/f00 /home/ren/rftp/data/source-tmpfs/d/d1-f00
ln -s /home/ren/iser/d1/f01 /home/ren/rftp/data/source-tmpfs/d/d1-f01
ln -s /home/ren/iser/d2/f00 /home/ren/rftp/data/source-tmpfs/d/d2-f00
ln -s /home/ren/iser/d2/f01 /home/ren/rftp/data/source-tmpfs/d/d2-f01

ln -s /home/ren/iser/d0/f00 /home/ren/rftp/data/sink-tmpfs/d/d0-f00
ln -s /home/ren/iser/d0/f01 /home/ren/rftp/data/sink-tmpfs/d/d0-f01
ln -s /home/ren/iser/d1/f00 /home/ren/rftp/data/sink-tmpfs/d/d1-f00
ln -s /home/ren/iser/d1/f01 /home/ren/rftp/data/sink-tmpfs/d/d1-f01
ln -s /home/ren/iser/d2/f00 /home/ren/rftp/data/sink-tmpfs/d/d2-f00
ln -s /home/ren/iser/d2/f01 /home/ren/rftp/data/sink-tmpfs/d/d2-f01
