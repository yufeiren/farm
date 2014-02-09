#! /bin/bash

source ./defination

if [ ! -x $Fio ]; then
        echo You need fio installed to start io evaluation
        exit 1
fi

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir

for nj in $njs
do
	for srv in 07 09 13
	do
		task=$Taskdir/srv365-$srv-fio-$nj
echo > "$task"
echo "[global]" >> "$task"
echo "time_based" >> "$task"
echo "runtime="$Runtime >> "$task"
echo "thread" >> "$task"
echo "direct=1" >> "$task"
echo "ioengine=sync"$ioengine >> "$task"
echo "rw=randread" >> "$task"
echo "group_reporting" >> "$task"
echo "randrepeat=0" >> "$task"
echo "size="$DataSize >> "$task"
echo "bs="$bs >> "$task"
echo "numjobs="$nj >> "$task"
echo "rate_iops="$iops >> "$task"
	done
echo "[/dev/sdi]" >> $Taskdir/srv365-07-fio-$nj
echo "[/dev/sdj]" >> $Taskdir/srv365-07-fio-$nj

echo "[/dev/sdc]" >> $Taskdir/srv365-09-fio-$nj
echo "[/dev/sdd]" >> $Taskdir/srv365-09-fio-$nj

echo "[/dev/sde]" >> $Taskdir/srv365-13-fio-$nj
echo "[/dev/sdf]" >> $Taskdir/srv365-13-fio-$nj

	touch $Logdir/fio-$nj.log

	$Fio --client=srv365-07.cewit.stonybrook.edu $Taskdir/srv365-07-fio-$nj --client=srv365-09.cewit.stonybrook.edu $Taskdir/srv365-09-fio-$nj --client=srv365-13.cewit.stonybrook.edu $Taskdir/srv365-13-fio-$nj > $Logdir/fio-$nj.log

	sleep 10

done

