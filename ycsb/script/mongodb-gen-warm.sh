#! /bin/bash

max=245
dbdir=/mnt/sdc/mongodb
dbname=t500g-fc10-fl1000

echo "[global]" > warm.job
echo "ioengine=sync" >> warm.job
echo "iodepth=1" >> warm.job
echo "bs=512k" >> warm.job
echo "rw=read" >> warm.job
echo "direct=1" >> warm.job
echo "" >> warm.job

echo "[j0]" >> warm.job
echo "filename="$dbdir/$dbname".0" >> warm.job
echo "" >> warm.job

i=1
while [ $i -le $max ]
do
        echo "[j"$i"]" >> warm.job
        echo "stonewall" >> warm.job
        echo "filename="$dbdir/$dbname"."$i >> warm.job
        echo "" >> warm.job

        (( i++ ))
done

