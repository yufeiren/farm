#! /bin/bash

source ./define.sh

for id in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
do
        rm -rf $sourcedir/bell_curve$id
        rm -rf $sinkdir/bell_curve$id
        mkdir -p $sourcedir/bell_curve$id
        mkdir -p $sinkdir/bell_curve$id
        i=0
        while [ $i -lt 113 ]
        do
	        # sink
                ln -s /dev/null $sinkdir/bell_curve$id/file_$i
                (( i++ ))
        done
done

