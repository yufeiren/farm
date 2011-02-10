#! /bin/bash

Fiotool=/home/ren/git/farm/fiotest
LocalDiskFileIn=/data/fiot/hundredin
LocalDiskFileOut=/data/fiot/hundredout
LustreDiskFileIn=/mnt/lustre/fiot/hundredin
LustreDiskFileOut=/mnt/lustre/fiot/hundredout
FileoutSize=100G
Logdir=/home/ren/fiotest

# ./fiotest -r/-w -o/-s -f /data/100Gtestfile2 -m 5M -i 1 -t 100M

# read file
for bufsiz in 1K 8K 32K 64K 128K 512K 1M 8M 32M 64M 128M
do
# local
	sleep 5;
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -f $LocalDiskFileIn -m $bufsiz -i 2 > $Logdir/RD_Local_M$bufsiz.txt 2>&1
	sleep 5;
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -s -f $LocalDiskFileIn -m $bufsiz -i 2 > $Logdir/RD_Stream_Local_M$bufsiz.txt 2>&1
	sleep 5;
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -o -f $LocalDiskFileIn -m $bufsiz -i 2 > $Logdir/RD_O_Direct_Local_M$bufsiz.txt 2>&1
# lustre
	sleep 5;
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -f $LustreDiskFileIn -m $bufsiz -i 2 > $Logdir/RD_Lustre_M$bufsiz.txt 2>&1
	sleep 5;
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -s -f $LustreDiskFileIn -m $bufsiz -i 2 > $Logdir/RD_Stream_Lustre_M$bufsiz.txt 2>&1
	sleep 5;
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -o -f $LustreDiskFileIn -m $bufsiz -i 2 > $Logdir/RD_O_Direct_Lustre_M$bufsiz.txt 2>&1
done

# write file
for bufsiz in 1K 8K 32K 64K 128K 512K 1M 8M 32M 64M 128M
do
# local
	sleep 5;
	rm -rf $LocalDiskFileOut
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -w -f $LocalDiskFileOut -m $bufsiz -t $FileoutSize -i 2 > $Logdir/WR_Local_M$bufsiz.txt 2>&1
	sleep 5;
	rm -rf $LocalDiskFileOut
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -s -f $LocalDiskFileOut -m $bufsiz -t $FileoutSize -i 2 > $Logdir/WR_Stream_Local_M$bufsiz.txt 2>&1
	sleep 5;
	rm -rf $LocalDiskFileOut
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -o -f $LocalDiskFileOut -m $bufsiz -t $FileoutSize -i 2 > $Logdir/WR_O_Direct_Local_M$bufsiz_WR.txt 2>&1
# lustre
	sleep 5;
	rm -rf $LustreDiskFileOut
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -f $LustreDiskFileOut -m $bufsiz -t $FileoutSize -i 2 > $Logdir/WR_Lustre_M$bufsiz.txt 2>&1
	sleep 5;
	rm -rf $LustreDiskFileOut
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -s -f $LustreDiskFileOut -m $bufsiz -t $FileoutSize -i 2 > $Logdir/WR_Stream_Lustre_M$bufsiz.txt 2>&1
	sleep 5;
	rm -rf $LustreDiskFileOut
	sync; echo 3 > /proc/sys/vm/drop_caches
	$Fiotool -r -o -f $LustreDiskFileOut -m $bufsiz -t $FileoutSize -i 2 > $Logdir/WR_O_Direct_Lustre_M$bufsiz.txt 2>&1
done
