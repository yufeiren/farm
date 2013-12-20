#! /bin/bash

mdadm --create /dev/md/md-raid0-b-l --level=0 --raid-devices=11 /dev/sdb1 /dev/sdc1 /dev/sdd1 /dev/sde1 /dev/sdf1 /dev/sdg1 /dev/sdh1 /dev/sdi1 /dev/sdj1 /dev/sdk1 /dev/sdl1

mdadm --create /dev/md/md-raid0-m-w --level=0 --raid-devices=11 /dev/sdm1 /dev/sdn1 /dev/sdo1 /dev/sdp1 /dev/sdq1 /dev/sdr1 /dev/sds1 /dev/sdt1 /dev/sdu1 /dev/sdv1 /dev/sdw1

mdadm --create /dev/md/md-raid0-x-aj --level=0 --raid-devices=12 /dev/sdx1 /dev/sdy1 /dev/sdz1 /dev/sdab1 /dev/sdac1 /dev/sdad1 /dev/sdae1 /dev/sdaf1 /dev/sdag1 /dev/sdah1 /dev/sdai1 /dev/sdaj1

mdadm --create /dev/md/md-raid0-ak-av --level=0 --raid-devices=12 /dev/sdak1 /dev/sdal1 /dev/sdam1 /dev/sdan1 /dev/sdao1 /dev/sdap1 /dev/sdaq1 /dev/sdar1 /dev/sdas1 /dev/sdat1 /dev/sdau1 /dev/sdav1


mdadm --query /dev/md/md-raid0-b-l
mdadm --query /dev/md/md-raid0-m-w
mdadm --query /dev/md/md-raid0-x-aj
mdadm --query /dev/md/md-raid0-ak-av


