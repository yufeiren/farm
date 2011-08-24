#! /bin/sh
# disk requirement 600GB disk

source ./defination

test -z "$test_root" || mkdir -p $test_root

# rftpmemory
test -z "$test_root/rftpmemory" || mkdir -p $test_root/rftpmemory

# rftpmemory/source
test -z "$test_root/rftpmemory/source" || mkdir -p $test_root/rftpmemory/source

cd $test_root/rftpmemory/source
ln -s /dev/zero zero

x=0;
while [ "$x" -ne 10 ];
do
	ln -s /dev/zero zero$x
	x=$(($x+1))
done

# rftpmemory/sink
test -z "$test_root/rftpmemory/sink" || mkdir -p $test_root/rftpmemory/sink

cd $test_root/rftpmemory/sink
ln -s /dev/null zero

x=0;
while [ "$x" -ne 10 ];
do
	ln -s /dev/null zero$x
	x=$(($x+1))
done

# rftpdisk
test -z "$test_root/disk" || mkdir -p $test_root/disk

# rftpdisk/source
test -z "$test_root/rftpdisk/source" || mkdir -p $test_root/rftpdisk/source

mkdir $test_root/rftpdisk/source/flist1
cd $test_root/rftpdisk/source/flist1
dd if=$rand_device of=f0 bs=10M count=10240

x=0;
mkdir $test_root/rftpdisk/source/flist10
cd $test_root/rftpdisk/source/flist10
while [ "$x" -ne 10 ];
do
	dd if=$rand_device of=f$x bs=10M count=1024
	x=$(($x+1))
done

x=0;
mkdir $test_root/rftpdisk/source/flist100
cd $test_root/rftpdisk/source/flist100
while [ "$x" -ne 100 ];
do
	dd if=$rand_device of=f$x bs=1M count=1024
	x=$(($x+1))
done

x=0;
mkdir $test_root/rftpdisk/source/flist1000
cd $test_root/rftpdisk/source/flist1000
while [ "$x" -ne 1000 ];
do
	dd if=$rand_device of=f$x bs=10M count=10
	x=$(($x+1))
done

x=0;
mkdir $test_root/rftpdisk/source/flist10000
cd $test_root/rftpdisk/source/flist10000
while [ "$x" -ne 10000 ];
do
	dd if=$rand_device of=f$x bs=1M count=10
	x=$(($x+1))
done

# rftpdisk/sink
test -z "$test_root/rftpdisk/sink" || mkdir -p $test_root/rftpdisk/sink


exit 0
