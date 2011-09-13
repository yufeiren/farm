#! /bin/bash

link_root=/home/ren/data/rftp

rm -rf $link_root/*

# link source memory
for i in a b c d e f g h
do
	test -z "$link_root/source/mem" || mkdir -p $link_root/source/mem
	ln -s /dev/zero $link_root/source/mem/zero$i
done

# link sink memory
for i in a b c d e f g h
do
	test -z "$link_root/sink/mem" || mkdir -p $link_root/sink/mem
	ln -s /dev/null $link_root/sink/mem/zero$i
	ln -s /dev/null $link_root/sink/mem/zerooneg$i
	ln -s /dev/null $link_root/sink/mem/zerohrdm$i
done

