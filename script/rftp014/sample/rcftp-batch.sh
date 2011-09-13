#! /bin/bash

env RCFTPRC=/home/ren/git/farm/script/rftp014/sample/rcftprc-rput-2M-256-direct rcftp -n -i -v < /home/ren/git/farm/script/rftp014/sample/mem2mem-rput-zeroa-ib0 > rput-zeroa.log 2>&1 &

env RCFTPRC=/home/ren/git/farm/script/rftp014/sample/rcftprc-rput-2M-256-direct rcftp -n -i -v < /home/ren/git/farm/script/rftp014/sample/mem2mem-rput-zerob-ib1 > rput-zerob.log 2>&1 &

env RCFTPRC=/home/ren/git/farm/script/rftp014/sample/rcftprc-rget-2M-400-direct rcftp -n -i -v < /home/ren/git/farm/script/rftp014/sample/mem2mem-rget-zeroc-ib0 > rget-zeroc.log 2>&1 & 

env RCFTPRC=/home/ren/git/farm/script/rftp014/sample/rcftprc-rget-2M-400-direct rcftp -n -i -v < /home/ren/git/farm/script/rftp014/sample/mem2mem-rget-zerod-ib1 > rget-zerod.log 2>&1 &


