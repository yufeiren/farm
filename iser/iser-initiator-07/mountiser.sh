#! /bin/bash

mount /dev/sdc /home/ren/iser/d0
mount /dev/sdd /home/ren/iser/d1
mount /dev/sde /home/ren/iser/d2
mount /dev/sdf /home/ren/iser/d3
mount /dev/sdg /home/ren/iser/d4
mount /dev/sdh /home/ren/iser/d5

chown -R ren:ren /home/ren/iser/d0
chown -R ren:ren /home/ren/iser/d1
chown -R ren:ren /home/ren/iser/d2
chown -R ren:ren /home/ren/iser/d3
chown -R ren:ren /home/ren/iser/d4
chown -R ren:ren /home/ren/iser/d5
