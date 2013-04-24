#! /bin/bash

source ./defination

if [ ! -x $Pm ]; then
        echo You need postmark installed to start evaluation
        exit 1
fi

test -d $Logdir || mkdir -p $Logdir
test -d $Taskdir || mkdir -p $Taskdir

# before start each test, cache has to be dropped

i=0
while [ $i -lt 4 ]; do
	task=$Taskdir/n${fnumber[$i]}-s${fsize[$i]}-t${trans[$i]}
	log=$Logdir/n${fnumber[$i]}-s${fsize[$i]}-t${trans[$i]}

	echo "set size "${fsize[$i]} > $task
	echo "set number "${fnumber[$i]} >> $task
	echo "set transactions "${trans[$i]} >> $task
	echo "set location "$loc >> $task
	echo "set read 524288" >> $task
	echo "set write 524288" >> $task
	echo "set bias read 10" >> $task
	echo "set bias create -1" >> $task
	echo "set buffering false" >> $task
	echo "show" >> $task
	echo "run" >> $task

	$Pm < $task > $log 2>&1

	(( i++ ))

done

