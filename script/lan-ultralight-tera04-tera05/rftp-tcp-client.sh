#! /bin/bash

source ./defination

if [ ! -x $Rcftp ]; then
	echo You need rcftp installed to start io evaluation
	exit 1
fi

echo > $Logdir/dateseq.log

# create config file
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
config=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum-$nc
touch $config
echo "cbufsiz="$cbufsiz > $config
echo "cbufnum="$cbufnum >> $config
echo "usesendfile=no" >> $config
echo "usesplice=no" >> $config
if [ $nc -eq 1 ]; then
	echo "devzerosiz=100G" >> $config
elif [ $nc -eq 2 ]; then
	echo "devzerosiz=50G" >> $config
elif [ $nc -eq 4 ]; then
	echo "devzerosiz=25G" >> $config
elif [ $nc -eq 8 ]; then
	echo "devzerosiz=12G" >> $config
elif [ $nc -eq 16 ]; then
	echo "devzerosiz=6G" >> $config
elif [ $nc -eq 32 ]; then
	echo "devzerosiz=3G" >> $config
fi
echo "rmtaddrnum=500" >> $config
echo "evbufnum=400" >> $config
echo "recvbufnum=500" >> $config
echo "rcstreamnum="$rcstreamnum >> $config
echo "readernum=2" >> $config
echo "writernum=2" >> $config
echo "ioengine=sync" >> $config
echo "directio=yes" >> $config
echo "rdma_qp_sq_depth = 500" >> $config
echo "rdma_qp_rq_depth = 500" >> $config
echo "rdma_cq_depth = 1000" >> $config
echo "wc_thread_num = 4" >> $config
echo "wc_event_num = 500" >> $config
				done
                done
        done
done

# task memory to memory
task=$Taskdir/mem2mem-rput-zero-tera04-tera05

echo "open "$ServIP $ServPort > $task
echo "user ftp ftp" >> $task
echo "bin" >> $task
echo "prompt" >> $task
echo "lcd "$SourceDir >> $task
echo "cd "$SinkDir >> $task
echo "rput zero" >> $task
echo "bye" >> $task


# start multitest test

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

pids=""
# start n clients
for nc in $numofclients
do
config=$Configdir/rcftp-$cbufsiz-$cbufnum-$rcstreamnum-$nc
	date "+%F %X" >> $Logdir/dateseq.log

	tmp=1
	while [ $tmp -le $nc ]
	do
	# log: $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-nc$nc-c$tmp-iter$i.log
	env RCFTPRC=$config $Rcftp -n -i -v < $task >> $Logdir/mem2mem-$cbufsiz-$cbufnum-$rcstreamnum-nc$nc-c$tmp-iter$i.log 2>&1 &
	pid=$!
	pids=$pids" "$pid
	(( tmp++ ))
	done
	wait $pids
done

sleep 5
                done
        done
done
(( i++ ))
done

