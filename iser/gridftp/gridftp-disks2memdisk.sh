#! /bin/bash

globus-url-copy -vb -nodcau file:///mnt/sdb/hg ftp://192.168.11.7:8011/home/ren/iser/d0/f00

for dir in source sink
do
	node=0
	ln -s /mnt/sdb/hg /home/ren/rftp/data/$dir-$node/d/d0-f00
	ln -s /mnt/sdc/hg /home/ren/rftp/data/$dir-$node/d/d0-f01
	ln -s /mnt/sdd/hg /home/ren/rftp/data/$dir-$node/d/d1-f00
	ln -s /mnt/sde/hg /home/ren/rftp/data/$dir-$node/d/d1-f01
	ln -s /mnt/sdf/hg /home/ren/rftp/data/$dir-$node/d/d2-f00
	ln -s /mnt/sdg/hg /home/ren/rftp/data/$dir-$node/d/d2-f01
	ln -s /mnt/sdh/hg /home/ren/rftp/data/$dir-$node/d2/d3-f00
	ln -s /mnt/sdi/hg /home/ren/rftp/data/$dir-$node/d2/d3-f01
	ln -s /mnt/sdj/hg /home/ren/rftp/data/$dir-$node/d2/d4-f00
	ln -s /mnt/sdk/hg /home/ren/rftp/data/$dir-$node/d2/d4-f01
	ln -s /mnt/sdl/hg /home/ren/rftp/data/$dir-$node/d2/d5-f00
	ln -s /mnt/sdm/hg /home/ren/rftp/data/$dir-$node/d2/d5-f01

	node=1
	ln -s /mnt/sdn/hg /home/ren/rftp/data/$dir-$node/d/d0-f00
	ln -s /mnt/sdo/hg /home/ren/rftp/data/$dir-$node/d/d0-f01
	ln -s /mnt/sdp/hg /home/ren/rftp/data/$dir-$node/d/d1-f00
	ln -s /mnt/sdq/hg /home/ren/rftp/data/$dir-$node/d/d1-f01
	ln -s /mnt/sdr/hg /home/ren/rftp/data/$dir-$node/d/d2-f00
	ln -s /mnt/sds/hg /home/ren/rftp/data/$dir-$node/d/d2-f01
	ln -s /mnt/sdt/hg /home/ren/rftp/data/$dir-$node/d2/d3-f00
	ln -s /mnt/sdu/hg /home/ren/rftp/data/$dir-$node/d2/d3-f01
	ln -s /mnt/sdv/hg /home/ren/rftp/data/$dir-$node/d2/d4-f00
	ln -s /mnt/sdw/hg /home/ren/rftp/data/$dir-$node/d2/d4-f01
	ln -s /mnt/sdx/hg /home/ren/rftp/data/$dir-$node/d2/d5-f00
	ln -s /mnt/sdy/hg /home/ren/rftp/data/$dir-$node/d2/d5-f01

	node=2
	ln -s /mnt/sdz/hg /home/ren/rftp/data/$dir-$node/d/d0-f00
	ln -s /mnt/sdab/hg /home/ren/rftp/data/$dir-$node/d/d0-f01
	ln -s /mnt/sdac/hg /home/ren/rftp/data/$dir-$node/d/d1-f00
	ln -s /mnt/sdad/hg /home/ren/rftp/data/$dir-$node/d/d1-f01
	ln -s /mnt/sdae/hg /home/ren/rftp/data/$dir-$node/d/d2-f00
	ln -s /mnt/sdaf/hg /home/ren/rftp/data/$dir-$node/d/d2-f01
	ln -s /mnt/sdag/hg /home/ren/rftp/data/$dir-$node/d2/d3-f00
	ln -s /mnt/sdah/hg /home/ren/rftp/data/$dir-$node/d2/d3-f01
	ln -s /mnt/sdai/hg /home/ren/rftp/data/$dir-$node/d2/d4-f00
	ln -s /mnt/sdaj/hg /home/ren/rftp/data/$dir-$node/d2/d4-f01
	ln -s /mnt/sdak/hg /home/ren/rftp/data/$dir-$node/d2/d5-f00
	ln -s /mnt/sdal/hg /home/ren/rftp/data/$dir-$node/d2/d5-f01

	node=3
	ln -s /mnt/sdam/hg /home/ren/rftp/data/$dir-$node/d/d0-f00
	ln -s /mnt/sdam/hg /home/ren/rftp/data/$dir-$node/d/d0-f01
	ln -s /mnt/sdan/hg /home/ren/rftp/data/$dir-$node/d/d1-f00
	ln -s /mnt/sdao/hg /home/ren/rftp/data/$dir-$node/d/d1-f01
	ln -s /mnt/sdap/hg /home/ren/rftp/data/$dir-$node/d/d2-f00
	ln -s /mnt/sdaq/hg /home/ren/rftp/data/$dir-$node/d/d2-f01
	ln -s /mnt/sdar/hg /home/ren/rftp/data/$dir-$node/d2/d3-f00
	ln -s /mnt/sdas/hg /home/ren/rftp/data/$dir-$node/d2/d3-f01
	ln -s /mnt/sdat/hg /home/ren/rftp/data/$dir-$node/d2/d4-f00
	ln -s /mnt/sdau/hg /home/ren/rftp/data/$dir-$node/d2/d4-f01
	ln -s /mnt/sdav/hg /home/ren/rftp/data/$dir-$node/d2/d5-f00
	ln -s /mnt/sdav/hg /home/ren/rftp/data/$dir-$node/d2/d5-f01

done

