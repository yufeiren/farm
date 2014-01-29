#! /bin/bash

# result is in ganglia
# memdisk to /dev/null

#ms="1 2 4 8 16 32"
ms="1"
cipher=aes256-cbc
dir=/home/ren/iser
rdir0=/home/ren/rftp/data/sink-0
rdir1=/home/ren/rftp/data/sink-1
filearr=( d0/f00 d0/f01 d1/f00 d1/f01 d2/f00 d2/f01 d3/f00 d3/f01 d4/f00 d4/f01 d5/f00 d5/f01 )
rfarr=( d/d0-f00 d/d0-f01 d/d1-f00 d/d1-f01 d/d2-f00 d/d2-f01 d2/d3-f00 d2/d3-f01 d2/d4-f00 d2/d4-f01 d2/d5-f00 d2/d5-f01)
period=600
dest=( 192.168.11.11 192.168.12.11 )

for m in $ms
do
	i=0
	while [ $i -lt $m ]
	do
		j=0
		while [ $j -lt ${#filearr[*]} ]
		do
			numactl --cpunodebind=0 --membind=0 scp -c $cipher $dir/${filearr[$j]} ren@${dest[0]}:$rdir0/${rfarr[$j]} &
			numactl --cpunodebind=1 --membind=1 scp -c $cipher $dir/${filearr[$j]} ren@${dest[1]}:$rdir1/${rfarr[$j]} &
			sleep 1
			(( j++ ))
		done
		(( i++ ))
	done

	sleep $period
	killall -s 9 scp
	sleep 5
done

