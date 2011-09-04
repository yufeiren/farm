#! /bin/bash

rcftp=/usr/bin/rcftp

if [ ! -x $rcftp ]; then
        echo You need rcftp installed to start io evaluation
        exit 1
fi

DataSize=100G
Rootdir=/home/ren/eval/v014
Logdir=$Rootdir/log
Configdir=$Rootdir/config
Taskdir=$Rootdir/task

ServIP0=192.168.1.17
ServIP1=192.168.2.17
ServPort=18519

rm -rf $Logdir
rm -rf $Taskdir
rm -rf $Configdir

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir
test -d $Configdir || mkdir -p $Configdir

# 4K 8K 16K 64K 128K 512K 1M"
cbufsizs="4096 8192 16384 65536 131072 524288 1048576"
cbufnums="1 8 16 64 128 256"
rcstreamnums="1 4 8 16"

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
config=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum
touch $config
echo "cbufsiz="$cbufsiz >> $config
echo "cbufnum="$cbufnum >> $config
echo "rmtaddrnum=500" >> $config
echo "evbufnum=400" >> $config
echo "recvbufnum=500" >> $config
echo "rcstreamnum="$rcstreamnum >> $config
echo "readernum=8" >> $config
echo "writernum=8" >> $config
echo "directio=yes" >> $config
if [ $cbufsiz -lt 524288 ]
then
	echo "devzerosiz=1G" >> $config
else
	echo "devzerosiz=10G" >> $config
fi
echo "rdma_qp_sq_depth = 1280" >> $config
echo "rdma_qp_rq_depth = 1280" >> $config
echo "rdma_cq_depth = 2000" >> $config
echo "wc_thread_num = 32" >> $config
echo "wc_event_num = 500" >> $config
		done
	done
done

# task memory to memory
task0=$Taskdir/mem2mem-rput-zeroa-ib0
task1=$Taskdir/mem2mem-rput-zerob-ib1

echo "open 192.168.1.17 18519" >> $task0
echo "user ftp ftp" >> $task0
echo "bin" >> $task0
echo "prompt" >> $task0
echo "lcd /home/ren/data/rftp/source/mem" >> $task0
echo "cd /home/ren/data/rftp/sink/mem" >> $task0
echo "rmput zeroa" >> $task0
echo "bye" >> $task0

echo "open 192.168.2.17 18519" >> $task1
echo "user ftp ftp" >> $task1
echo "bin" >> $task1
echo "prompt" >> $task1
echo "lcd /home/ren/data/rftp/source/mem" >> $task1
echo "cd /home/ren/data/rftp/sink/mem" >> $task1
echo "rmput zerob" >> $task1
echo "bye" >> $task1

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
touch $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib0.log
touch $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib1.log

date > $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib0.log
date > $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib1.log

env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $task0 >> $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib0.log &
pid0=$!
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $task1 >> $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib1.log &
pid1=$!
wait $pid0 $pid1
		done
	done
done

# task disk to memory 1+1 2+2 4+4 8+8
task10=$Taskdir/disk2mem-rput-zeroa-ib0
task11=$Taskdir/disk2mem-rput-zeroe-ib1
task20=$Taskdir/disk2mem-rmput-zeroab-ib0
task21=$Taskdir/disk2mem-rmput-zeroef-ib1
task40=$Taskdir/disk2mem-rmput-zeroabcd-ib0
task41=$Taskdir/disk2mem-rmput-zeroefgh-ib1
task80=$Taskdir/disk2mem-rmput-zeroabcdefgh-ib0
task81=$Taskdir/disk2mem-rmput-zeroabcdefgh-ib1

echo "open 192.168.1.17 18519" >> $task0
echo "user ftp ftp" >> $task0
echo "bin" >> $task0
echo "prompt" >> $task0
echo "lcd /home/ren/data/rftp/source/mem" >> $task0
echo "cd /home/ren/data/rftp/sink/mem" >> $task0
echo "rmput zeroa" >> $task0
echo "bye" >> $task0

echo "open 192.168.2.17 18519" >> $task1
echo "user ftp ftp" >> $task1
echo "bin" >> $task1
echo "prompt" >> $task1
echo "lcd /home/ren/data/rftp/source/mem" >> $task1
echo "cd /home/ren/data/rftp/sink/mem" >> $task1
echo "rmput zerob" >> $task1
echo "bye" >> $task1

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
touch $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib0.log
touch $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib1.log

env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $task0 > $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib0.log &
pid0=$!
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $task1 > $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-ib1.log &
pid1=$!
wait $pid0 $pid1
		done
	done
done

