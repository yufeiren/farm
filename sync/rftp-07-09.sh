#! /bin/bash

mkdir -p ~/sync/rftp

cp -R ~/git/rftp/netkit-ftp ~/sync/rftp
cp -R ~/git/rftp/linux-ftpd ~/sync/rftp

tar cvf ~/sync/rftp.tar ~/sync/rftp

scp ~/sync/rftp.tar ren@srv365-07.cewit.stonybrook.edu:~/sync
scp ~/sync/rftp.tar ren@srv365-09.cewit.stonybrook.edu:~/sync

