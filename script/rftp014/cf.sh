#! /bin/sh

link_root=/home/ren/data/rftp
data_root=/mnt/fio

# create disk files
for i in a b c d e f g h
do
	test -z "$data_root$i/source" || mkdir -p $data_root$i/source
	dd if=/dev/zero of=$data_root$i/source/zero$i bs=1M count=10240

	test -z "$data_root$i/sink" || mkdir -p $data_root$i/sink
	touch $data_root$i/sink/zero$i
done

# link source memory
for i in a b c d e f g h
do
	test -z "$link_root/source/mem" || mkdir -p $link_root/source/mem
	ln -s /dev/zero $link_root/source/mem/zero$i
done

# link source disk
for i in a b c d e f g h
do
	test -z "$link_root/source/disk" || mkdir -p $link_root/source/disk
	ln -s $data_root$i/source/zero$i $link_root/source/disk/zero$i
done

# link sink memory
for i in a b c d e f g h
do
	test -z "$link_root/sink/mem" || mkdir -p $link_root/sink/mem
	ln -s /dev/null $link_root/sink/mem/zero$i
done

# link sink disk
for i in a b c d e f g h
do
	test -z "$link_root/sink/disk" || mkdir -p $link_root/sink/disk
	ln -s $data_root$i/sink/zero$i $link_root/sink/disk/zero$i
done

