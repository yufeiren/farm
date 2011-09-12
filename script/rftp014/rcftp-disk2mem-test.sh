#! /bin/bash

rcftp=/usr/bin/rcftp

if [ ! -x $rcftp ]; then
        echo You need rcftp installed to start io evaluation
        exit 1
fi

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

# "4K 8K 16K 32K 64K 128K 256K 512K 1M 2M"
cbufsizs="4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152"
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
if [ $cbufsiz -lt 524288 ]; then
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

# task disk to memory 1+1 2+2 4+4 8+8
for diskfile in zero zerooneg zerohrdm
do
	task10=$Taskdir/disk2mem-rmput-$diskfile-a-ib0
	task11=$Taskdir/disk2mem-rmput-$diskfile-e-ib1
	task20=$Taskdir/disk2mem-rmput-$diskfile-ab-ib0
	task21=$Taskdir/disk2mem-rmput-$diskfile-ef-ib1
	task40=$Taskdir/disk2mem-rmput-$diskfile-abcd-ib0
	task41=$Taskdir/disk2mem-rmput-$diskfile-efgh-ib1
	task80=$Taskdir/disk2mem-rmput-$diskfile-abcdefgh-ib0
	task81=$Taskdir/disk2mem-rmput-$diskfile-abcdefgh-ib1

echo "open 192.168.1.17 18519" >> $task10
echo "open 192.168.1.17 18519" >> $task20
echo "open 192.168.1.17 18519" >> $task40
echo "open 192.168.1.17 18519" >> $task80
echo "open 192.168.2.17 18519" >> $task11
echo "open 192.168.2.17 18519" >> $task21
echo "open 192.168.2.17 18519" >> $task41
echo "open 192.168.2.17 18519" >> $task81

echo "user ftp ftp" >> $task10
echo "user ftp ftp" >> $task11
echo "user ftp ftp" >> $task20
echo "user ftp ftp" >> $task21
echo "user ftp ftp" >> $task40
echo "user ftp ftp" >> $task41
echo "user ftp ftp" >> $task80
echo "user ftp ftp" >> $task81

echo "bin" >> $task10
echo "bin" >> $task11
echo "bin" >> $task20
echo "bin" >> $task21
echo "bin" >> $task40
echo "bin" >> $task41
echo "bin" >> $task80
echo "bin" >> $task81

echo "prompt" >> $task10
echo "prompt" >> $task11
echo "prompt" >> $task20
echo "prompt" >> $task21
echo "prompt" >> $task40
echo "prompt" >> $task41
echo "prompt" >> $task80
echo "prompt" >> $task81

echo "lcd /home/ren/data/rftp/source/disk" >> $task10
echo "lcd /home/ren/data/rftp/source/disk" >> $task11
echo "lcd /home/ren/data/rftp/source/disk" >> $task20
echo "lcd /home/ren/data/rftp/source/disk" >> $task21
echo "lcd /home/ren/data/rftp/source/disk" >> $task40
echo "lcd /home/ren/data/rftp/source/disk" >> $task41
echo "lcd /home/ren/data/rftp/source/disk" >> $task80
echo "lcd /home/ren/data/rftp/source/disk" >> $task81

echo "cd /home/ren/data/rftp/sink/mem" >> $task10
echo "cd /home/ren/data/rftp/sink/mem" >> $task11
echo "cd /home/ren/data/rftp/sink/mem" >> $task20
echo "cd /home/ren/data/rftp/sink/mem" >> $task21
echo "cd /home/ren/data/rftp/sink/mem" >> $task40
echo "cd /home/ren/data/rftp/sink/mem" >> $task41
echo "cd /home/ren/data/rftp/sink/mem" >> $task80
echo "cd /home/ren/data/rftp/sink/mem" >> $task81

printf "rmput %sa\n" $diskfile >> $task10
printf "rmput %se\n" $diskfile >> $task11
printf "rmput %sa %sb\n" $diskfile $diskfile >> $task20
printf "rmput %se %sf\n" $diskfile $diskfile >> $task21
printf "rmput %sa %sb %sc %sd\n" $diskfile $diskfile $diskfile $diskfile >> $task40
printf "rmput %se %sf %sg %sh\n" $diskfile $diskfile $diskfile $diskfile >> $task41
printf "rmput %sa %sb %sc %sd %se %sf %sg %sh\n" $diskfile $diskfile $diskfile $diskfile $diskfile $diskfile $diskfile $diskfile >> $task80
printf "rmput %sa %sb %sc %sd %se %sf %sg %sh\n" $diskfile $diskfile $diskfile $diskfile $diskfile $diskfile $diskfile $diskfile >> $task81

echo "bye" >> $task10
echo "bye" >> $task11
echo "bye" >> $task20
echo "bye" >> $task21
echo "bye" >> $task40
echo "bye" >> $task41
echo "bye" >> $task80
echo "bye" >> $task81

done


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
			for filenum in 1 2 4 8
			do

touch $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib0.log
touch $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib1.log

date > $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib0.log
date > $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib1.log

if [ $cbufsiz -lt 524288 ]; then
	filescale=zerooneg
else
	filescale=zero
fi

if [ $filenum -eq 1 ]; then
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $Taskdir/disk2mem-rmput-$filescale-a-ib0 >> $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib0.log &
pid0=$!
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $Taskdir/disk2mem-rmput-$filescale-e-ib1 >> $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib1.log &
pid1=$!
elif [ $filenum -eq 2 ]; then
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $Taskdir/disk2mem-rmput-$filescale-ab-ib0 >> $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib0.log &
pid0=$!
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $Taskdir/disk2mem-rmput-$filescale-ef-ib1 >> $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib1.log &
pid1=$!
elif [ $filenum -eq 4 ]; then
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $Taskdir/disk2mem-rmput-$filescale-abcd-ib0 >> $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib0.log &
pid0=$!
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $Taskdir/disk2mem-rmput-$filescale-efgh-ib1 >> $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib1.log &
pid1=$!
elif [ $filenum -eq 8 ]; then
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $Taskdir/disk2mem-rmput-$filescale-abcdefgh-ib0 >> $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib0.log &
pid0=$!
env RCFTPRC=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum $rcftp -n -i -v < $Taskdir/disk2mem-rmput-$filescale-abcdefgh-ib1 >> $Logdir/disk2mem-$cbufsiz-$cbufnum-$rcstreamnum-f$filenum-ib1.log &
pid1=$!
else
	echo "error filenum!!!"
	exit 1
fi

wait $pid0 $pid1

sleep 10
			done
		done
	done
done

