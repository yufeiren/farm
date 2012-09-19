#! /bin/bash

source ./defination

if [ ! -x $Rcftp ]; then
	echo You need rcftp installed to start io evaluation
	exit 1
fi

test -d $Configdir || mkdir -p $Configdir
test -d $Taskdir || mkdir -p $Taskdir
test -d $Logdir || mkdir -p $Logdir

# create config file
for cbufsiz in $cbufsizs
do
	for rcstreamnum in $rcstreamnums
	do
config=$Configdir/rcftp-$cbufsiz-$rcstreamnum
touch $config
echo "cbufsiz="$cbufsiz > $config
echo "maxbufpoolsiz="$maxbufpoolsiz >> $config
echo "devzerosiz=700G" >> $config
echo "rcstreamnum="$rcstreamnum >> $config
echo "readernum=2" >> $config
echo "writernum=2" >> $config
echo "ioengine=sync" >> $config
echo "directio=yes" >> $config
echo "rdma_qp_sq_depth = 1280" >> $config
echo "rdma_qp_rq_depth = 1280" >> $config
echo "rdma_cq_depth = 2000" >> $config
echo "wc_thread_num = 4" >> $config
		done
	done
done

# task memory to memory
task=$Taskdir/mem2mem-rput-zerodir

echo "open "$ServIP $ServPort > $task
echo "user ftp ftp" >> $task
echo "bin" >> $task
echo "prompt" >> $task
echo "lcd "$SourceDir >> $task
echo "cd "$SinkDir >> $task
echo "rput zerodir" >> $task
echo "bye" >> $task


for cbufsiz in $cbufsizs
do
	for rcstreamnum in $rcstreamnums
	do
		config=$Configdir/rcftp-$cbufsiz-$rcstreamnum

		numactl --cpunodebind=1 --membind=1 env RCFTPRC=$config $Rcftp -n -i -v < $task > $Logdir/mem2mem-$cbufsiz-$rcstreamnum.log 2>&1

		sleep 5
	done
done

