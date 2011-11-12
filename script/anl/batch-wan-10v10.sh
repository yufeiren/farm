#! /bin/bash

source ./define.sh

# direct io
for c in $cbufsizs
do
        for s in $rcstreamnums
        do
cmd1="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.1-bc1 >> $logdir/rcftp-directio-bc1-$c-d$s.log"
cmd2="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.2-bc2 >> $logdir/rcftp-directio-bc2-$c-d$s.log"
cmd3="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.4-bc3 >> $logdir/rcftp-directio-bc3-$c-d$s.log"
cmd4="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.5-bc4 >> $logdir/rcftp-directio-bc4-$c-d$s.log"
cmd5="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.6-bc5 >> $logdir/rcftp-directio-bc5-$c-d$s.log"
cmd6="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.7-bc6 >> $logdir/rcftp-directio-bc6-$c-d$s.log"
cmd7="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.8-bc7 >> $logdir/rcftp-directio-bc7-$c-d$s.log"
cmd8="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.9-bc8 >> $logdir/rcftp-directio-bc8-$c-d$s.log"
cmd9="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.10-bc9 >> $logdir/rcftp-directio-bc9-$c-d$s.log"
cmd10="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.11-bc10 >> $logdir/rcftp-directio-bc10-$c-d$s.log"
cmd11="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.12-bc11 >> $logdir/rcftp-directio-bc11-$c-d$s.log"
cmd12="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.13-bc12 >> $logdir/rcftp-directio-bc12-$c-d$s.log"
cmd13="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.14-bc13 >> $logdir/rcftp-directio-bc13-$c-d$s.log"
cmd14="env RCFTPRC=$configdir/rcftprc-nondirectio-$c-$s $rcftpbin -n -i -v < $taskdir/get-10.200.200.15-bc14 >> $logdir/rcftp-directio-bc14-$c-d$s.log"

echo "#! /bin/bash" > get-cvrani01.sh
echo "#! /bin/bash" > get-cvrani02.sh
echo "#! /bin/bash" > get-cvrani04.sh
echo "#! /bin/bash" > get-cvrani05.sh
echo "#! /bin/bash" > get-cvrani06.sh
echo "#! /bin/bash" > get-cvrani07.sh
echo "#! /bin/bash" > get-cvrani08.sh
echo "#! /bin/bash" > get-cvrani09.sh
echo "#! /bin/bash" > get-cvrani10.sh
echo "#! /bin/bash" > get-cvrani11.sh
echo "#! /bin/bash" > get-cvrani12.sh
echo "#! /bin/bash" > get-cvrani13.sh
echo "#! /bin/bash" > get-cvrani14.sh
echo "#! /bin/bash" > get-cvrani15.sh

echo "ssh $username@as0171" \"$cmd1\" >> get-cvrani01.sh
echo "ssh $username@as0172" \"$cmd2\" >> get-cvrani02.sh
echo "ssh $username@as0173" \"$cmd3\" >> get-cvrani04.sh
echo "ssh $username@as0174" \"$cmd4\" >> get-cvrani05.sh
echo "ssh $username@as0175" \"$cmd5\" >> get-cvrani06.sh
echo "ssh $username@as0176" \"$cmd6\" >> get-cvrani07.sh
echo "ssh $username@as0178" \"$cmd7\" >> get-cvrani08.sh
echo "ssh $username@as0179" \"$cmd8\" >> get-cvrani09.sh
echo "ssh $username@as0180" \"$cmd9\" >> get-cvrani10.sh
echo "ssh $username@as0181" \"$cmd10\" >> get-cvrani11.sh
echo "ssh $username@as0182" \"$cmd11\" >> get-cvrani12.sh
echo "ssh $username@as0183" \"$cmd12\" >> get-cvrani13.sh
echo "ssh $username@as0184" \"$cmd13\" >> get-cvrani14.sh
echo "ssh $username@as0185" \"$cmd14\" >> get-cvrani15.sh

chmod +x get-*.sh


date "+%F %X" >> $logdir/datetime_start.log

./get-cvrani01.sh &
pid1=$!
./get-cvrani02.sh &
pid2=$!
./get-cvrani04.sh &
pid3=$!
./get-cvrani05.sh &
pid4=$!
./get-cvrani06.sh &
pid5=$!
./get-cvrani07.sh &
pid6=$!
./get-cvrani08.sh &
pid7=$!
./get-cvrani09.sh &
pid8=$!
./get-cvrani10.sh &
pid9=$!
./get-cvrani11.sh &
pid10=$!
./get-cvrani12.sh &
pid11=$!
./get-cvrani13.sh &
pid12=$!
./get-cvrani14.sh &
pid13=$!
./get-cvrani15.sh &
pid14=$!

wait $pid1 $pid2 $pid3 $pid4 $pid5 $pid6 $pid7 $pid8 $pid9 $pid10 $pid11 $pid12 $pid13 $pid14
date "+%F %X" >> $logdir/datetime_end.log

sleep 10
	done
done

