#! /bin/bash

# evaluate the tgt performance of 4 nodes NUMA host: srv365-11

source ./defination

if [ ! -x $Fio ]; then
        echo You need fio installed to start io evaluation
        exit 1
fi

rm -rf $Logdir
rm -rf $Taskdir

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir
test -e $LogFile || touch $LogFile

# throughput

# 07 logout
iscsiadm -m node --logoutall=all
# 11 restart tgtd
ssh root@srv365-11.cewit.stonybrook.edu "echo 3 > /proc/sys/vm/drop_caches"
ssh root@srv365-11.cewit.stonybrook.edu "bash /home/ren/iser/tgtd-startup-multiproc.sh"
# 07 login
iscsiadm -m node --loginall=all

for size in $sizes
do
	for rw in $rws
	do
		for bs in $bss
		do
ssh root@srv365-11.cewit.stonybrook.edu "echo 3 > /proc/sys/vm/drop_caches"

fio --minimal --rw=$rw --size=$size --ioengine=libaio --iodepth=8 --direct=1 --bs=$bs --name=w1 --filename=/dev/sdd --name=w2 --filename=/dev/sde --name=w3 --filename=/dev/sdf --name=w4 --filename=/dev/sdc >> $LogFile
		done
	done
done

