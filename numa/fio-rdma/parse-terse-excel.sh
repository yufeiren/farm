#! /bin/sh

source ./defination

for file in terse-cpusys.log terse-cpuuser.log terse-write-bandwidth.log
do
for start in 1 2 3 4 5 6 7 8
do
cmd="cat "$file" | sed -n "
i=$start
while [ $i -lt 201 ]
do
        t="-e "
        t=$t$i
        t=$t"p "
        cmd=$cmd$t
        i=$(expr $i + 8)
done
cmd=$cmd" > ./"$file-sec$start
echo $cmd
done
done
