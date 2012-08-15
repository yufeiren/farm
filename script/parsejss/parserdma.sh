#! /bin/bash

source ./defination

test -d $Parsedir || mkdir -p $Parsedir

i=0
while [ $i -lt $Iter ]
do
for cbufsiz in $cbufsizs
do
        for cbufnum in $cbufnums
        do
                for rcstreamnum in $rcstreamnums
                do
                        if [ $cbufnum -lt $rcstreamnum ]
                        then
                                continue
                        fi

for nc in $numofclients
do
	tmp=1
	while [ $tmp -le $nc ]
	do

Logfile=$Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-nc$nc-c$tmp-iter$i.log

tail -2 $Logfile | head -1 | cut -f 1 -d ' ' >> $Parsedir/nc$nc-size.log
tail -2 $Logfile | head -1 | cut -f 5 -d ' ' >> $Parsedir/nc$nc-time.log

	(( tmp++ ))
	done
done

                done
        done
done
(( i++ ))
done

