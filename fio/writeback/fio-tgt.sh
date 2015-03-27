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

cat /dev/null > $Logdir/$TestSuite-thr.log
cat /dev/null > $Logdir/$TestSuite-lat.log

# throughput

setup_pagecache_default() {
	# 07 logout
	iscsiadm -m node --logoutall=all
	# 11 restart tgtd
	ssh root@srv365-11.cewit.stonybrook.edu "echo 3 > /proc/sys/vm/drop_caches"
	ssh root@srv365-11.cewit.stonybrook.edu "killall -s 9 tgtd"
	ssh root@srv365-11.cewit.stonybrook.edu "bash /home/ren/iser/tgtd-default.sh"
	ssh root@srv365-11.cewit.stonybrook.edu "bash /home/ren/iser/tgtd-startup-multiproc.sh"
	# 07 login
	iscsiadm -m node --loginall=all
}

setup_pagecache_numa() {
	# 07 logout
	iscsiadm -m node --logoutall=all
	# 11 restart tgtd
	ssh root@srv365-11.cewit.stonybrook.edu "echo 3 > /proc/sys/vm/drop_caches"
	ssh root@srv365-11.cewit.stonybrook.edu "killall -s 9 tgtd"
	ssh root@srv365-11.cewit.stonybrook.edu "bash /home/ren/iser/tgtd-numa.sh"
	ssh root@srv365-11.cewit.stonybrook.edu "bash /home/ren/iser/tgtd-startup-multiproc.sh"
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
ssh root@srv365-11.cewit.stonybrook.edu "echo 3 > /proc/sys/vm/drop_caches"
sleep 3
# throughput eval
fio --minimal --rw=$rw --size=$size --ioengine=libaio --iodepth=8 --direct=1 --bs=$bs --name=w1 --filename=/dev/sdd --name=w2 --filename=/dev/sde --name=w3 --filename=/dev/sdf --name=w4 --filename=/dev/sdc >> $Logdir/$TestSuite-thr.log

ssh root@srv365-11.cewit.stonybrook.edu "echo 3 > /proc/sys/vm/drop_caches"
sleep 3
# latency eval
fio --minimal --rw=$rw --size=$size --ioengine=sync --iodepth=1 --direct=1 --bs=$bs --name=w1 --filename=/dev/sdd --name=w2 --filename=/dev/sde --name=w3 --filename=/dev/sdf --name=w4 --filename=/dev/sdc >> $Logdir/$TestSuite-lat.log
		done
	done
done
}

setup_pagecache_default
run_test

sleep 3

setup_pagecache_numa
run_test
