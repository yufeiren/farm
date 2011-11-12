#! /bin/bash

# create_file
mkdir -p /scratch/yren/rftp/source
mkdir -p /scratch/yren/rftp/sink

for id in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
do
	mkdir -p /scratch/yren/rftp/source/bell_curve$id
	mkdir -p /scratch/yren/rftp/sink/bell_curve$id

	i=0
        while [ $i -lt 113 ]
        do
		touch /scratch/yren/rftp/sink/bell_curve$id/file_$i
		(( i++ ))
        done
done

