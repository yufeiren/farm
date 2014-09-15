#! /bin/bash

source ./defination
mkdir -p $Logdir
mkdir -p $Taskdir
echo > $Logdir/load.log
echo > $Logdir/run.log

for thread in $threads
do
	for fieldcount in $fieldcounts
	do
		for fieldlength in $fieldlengths
		do
			i=0
			while [ $i -lt ${#readproportions[*]} ]
			do
				workload=$Taskdir/wl-$fieldlength-$fieldcount-${readproportions[$i]}-${updateproportions[$i]}-$thread.workload
				touch $workload
				echo > $workload
				table=$Testcase-$fieldlength-$fieldcount-${readproportions[$i]}-${updateproportions[$i]}-$thread

echo "fieldcount="$fieldcount >> $workload
echo "fieldlength="$fieldlength >> $workload
echo "recordcount="$recordcount >> $workload
echo "operationcount="$operationcount >> $workload
echo "workload=com.yahoo.ycsb.workloads.CoreWorkload" >> $workload
echo "readallfields=true" >> $workload
echo "readproportion="${readproportions[$i]} >> $workload
echo "updateproportion="${updateproportions[$i]} >> $workload
echo "scanproportion=0" >> $workload
echo "insertproportion=0" >> $workload
echo "requestdistribution=uniform" >> $workload
echo "mongodb.url="$DBurl >> $workload
echo "mongodb.database="$DBname >> $workload
echo "mongodb.writeConcern=safe" >> $workload
echo "mongodb.maxconnections=10" >> $workload
echo "table=$table" >> $workload
echo "" >> $workload
# for ssl
echo "mongodb.ssl="$SSLenable >> $workload
echo "mongodb.keyStore="$keyStore >> $workload
echo "mongodb.trustStore="$trustStore >> $workload
echo "mongodb.keyStorePassword="$keyStorePassword >> $workload
echo "mongodb.trustStorePassword="$trustStorePassword >> $workload

				# load ycsb
				$YCSB load mongodb -P $workload -s -threads $thread >> $Logdir/load.log
				# run ycsb
				$YCSB run mongodb -P $workload -s -threads $thread >> $Logdir/run.log
			(( i++ ))
			done
		done
	done
done

