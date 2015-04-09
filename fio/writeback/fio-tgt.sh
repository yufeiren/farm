#! /bin/bash

# evaluate the tgt performance of 4 nodes NUMA host: srv365-11

source ./defination

if [ ! -x $Fio ]; then
        echo You need fio installed to start io evaluation
        exit 1
fi

rm -rf $Logdir
rm -rf $Taskdir
rm -rf $JobDir

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir
test -d $JobDir || mkdir -p $JobDir
test -e $LogFile || touch $LogFile

cat /dev/null > $LogFile

# throughput
clean_pagecache() {
	# clean page cache
	ssh root@srv365-11.cewit.stonybrook.edu "echo 3 > /proc/sys/vm/drop_caches"
}

setup_pagecache_numa() {
	# 07 logout
	iscsiadm -m node --logoutall=all
	# 11 restart tgtd
	ssh root@srv365-11.cewit.stonybrook.edu "echo 3 > /proc/sys/vm/drop_caches"
	ssh root@srv365-11.cewit.stonybrook.edu "killall -s 9 tgtd"
	ssh root@srv365-11.cewit.stonybrook.edu "bash /home/ren/iser/tgtd-numa.sh"
	ssh root@srv365-11.cewit.stonybrook.edu "cd /home/ren/iser/ && bash /home/ren/iser/tgtd-startup-multiproc.sh"
	# 07 login
	iscsiadm -m node --loginall=all
}

run_test() {
for size in $sizes
do
	for rw in $rws
	do
		for bs in $bss
		do
clean_pagecache

sleep 3
# throughput eval
jobfile=$JobDir/$size-$rw-$bs
touch $jobfile && cat /dev/null > $jobfile
echo "[global]" > $jobfile
echo "rw="$rw >> $jobfile
echo "size="$size >> $jobfile
echo "bs="$bs >> $jobfile
echo "ioengine=libaio" >> $jobfile
echo "iodepth=8" >> $jobfile
echo "direct=1" >> $jobfile
echo "[mywrite]" >> $jobfile
echo "filename=/dev/sdc" >> $jobfile

# fio --minimal --rw=$rw --size=$size --ioengine=libaio --iodepth=8 --direct=1 --bs=$bs --name=w1 --filename=/dev/sdd --name=w2 --filename=/dev/sde --name=w3 --filename=/dev/sdf --name=w4 --filename=/dev/sdc >> $Logdir/$TestSuite-thr.log

fio --minimal --client=srv365-13.cewit.stonybrook.edu $jobfile --client=srv365-15.cewit.stonybrook.edu $jobfile >> $LogFile

sleep 2
		done
	done
done
}

run_test

