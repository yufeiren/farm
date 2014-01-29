#! /bin/bash

# result is in ganglia
# memdisk to memdisk

ms="1 2 4 8 16 32"
cipher=aes128-cbc
dir=/home/ren/iser
filearr=( d0/f00 d0/f01 d1/f00 d1/f01 d2/f00 d2/f01 d3/f00 d3/f01 d4/f00 d4/f01 d5/f00 d5/f01 )
period=300
dest=( 192.168.1.9 192.168.2.9 )

for m in $ms
do
	i=0
	while [ $i -lt $m ]
	do
		j=0
		while [ $j -lt ${#filearr[*]} ]
		do
			numactl --cpunodebind=0 --membind=0 scp -c $cipher $dir/${filearr[$j]} ren@${dest[0]}:$dir/${filearr[$j]} &
			numactl --cpunodebind=1 --membind=1 scp -c $cipher $dir/${filearr[$j]} ren@${dest[1]}:$dir/${filearr[$j]} &
			sleep 1
			(( j++ ))
		done
		(( i++ ))
	done

	sleep $period
	killall -s 9 scp
	sleep 5
done

