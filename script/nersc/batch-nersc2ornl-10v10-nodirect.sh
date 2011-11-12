#! /bin/bash

source ./define.sh

# direct io
for c in $cbufsizs
do
        for s in $rcstreamnums
	do
cmd1="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.93-bc1 > $logdir/rcftp-nondirectio-bc1-$c-d$s.log"
cmd2="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.94-bc2 > $logdir/rcftp-nondirectio-bc2-$c-d$s.log"
cmd3="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.95-bc3 > $logdir/rcftp-nondirectio-bc3-$c-d$s.log"
cmd4="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.96-bc4 > $logdir/rcftp-nondirectio-bc4-$c-d$s.log"
cmd5="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.97-bc5 > $logdir/rcftp-nondirectio-bc5-$c-d$s.log"
cmd6="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.98-bc6 > $logdir/rcftp-nondirectio-bc6-$c-d$s.log"
cmd7="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.99-bc7 > $logdir/rcftp-nondirectio-bc7-$c-d$s.log"
cmd8="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.100-bc8 > $logdir/rcftp-nondirectio-bc8-$c-d$s.log"
cmd9="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.101-bc9 > $logdir/rcftp-nondirectio-bc9-$c-d$s.log"
cmd10="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/put-10.200.200.102-bc10 > $logdir/rcftp-nondirectio-bc10-$c-d$s.log"

echo "#! /bin/bash" > put-$ornl01.sh
echo "#! /bin/bash" > put-$ornl02.sh
echo "#! /bin/bash" > put-$ornl03.sh
echo "#! /bin/bash" > put-$ornl04.sh
echo "#! /bin/bash" > put-$ornl05.sh
echo "#! /bin/bash" > put-$ornl06.sh
echo "#! /bin/bash" > put-$ornl07.sh
echo "#! /bin/bash" > put-$ornl08.sh
echo "#! /bin/bash" > put-$ornl09.sh
echo "#! /bin/bash" > put-$ornl10.sh

echo "ssh $username@$cvrani01" \"$cmd1\" >> put-$ornl01.sh
echo "ssh $username@$cvrani02" \"$cmd2\" >> put-$ornl02.sh
echo "ssh $username@$cvrani04" \"$cmd3\" >> put-$ornl03.sh
echo "ssh $username@$cvrani05" \"$cmd4\" >> put-$ornl04.sh
echo "ssh $username@$cvrani06" \"$cmd5\" >> put-$ornl05.sh
echo "ssh $username@$cvrani07" \"$cmd6\" >> put-$ornl06.sh
echo "ssh $username@$cvrani08" \"$cmd7\" >> put-$ornl07.sh
echo "ssh $username@$cvrani09" \"$cmd8\" >> put-$ornl08.sh
echo "ssh $username@$cvrani10" \"$cmd9\" >> put-$ornl09.sh
echo "ssh $username@$cvrani11" \"$cmd10\" >> put-$ornl10.sh

chmod +x put-*.sh

date "+%F %X" >> $logdir/datetime_start.log

./put-$ornl01.sh &
pid1=$!
./put-$ornl02.sh &
pid2=$!
./put-$ornl03.sh &
pid3=$!
./put-$ornl04.sh &
pid4=$!
./put-$ornl05.sh &
pid5=$!
./put-$ornl06.sh &
pid6=$!
./put-$ornl07.sh &
pid7=$!
./put-$ornl08.sh &
pid8=$!
./put-$ornl09.sh &
pid9=$!
./put-$ornl10.sh &
pid10=$!

wait $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7 $pid8 $pid9 $pid10
date "+%F %X" >> $logdir/datetime_end.log

sleep 10
	done
done

