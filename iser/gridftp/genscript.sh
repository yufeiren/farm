#! /bin/bash

source ./defination

touch all.sh
chmod +x all.sh
echo "#! /bin/bash" > all.sh

i=0
while [ $i -lt ${#dstarr[*]} ]
do
touch ${dstarr[$i]}.sh
chmod +x ${dstarr[$i]}.sh
echo "#! /bin/bash" > ${dstarr[$i]}.sh
echo "GLOBUS_LOCATION=/usr/local/globus" >> ${dstarr[$i]}.sh
echo ". $GLOBUS_LOCATION/etc/globus-user-env.sh" >> ${dstarr[$i]}.sh
echo "while [ 1 ]; do" >> ${dstarr[$i]}.sh
echo "globus-url-copy -vb -nodcau file:///home/ren/iser/${srcarr[$i]} ftp://${iparr[$i]}:${portarr[$i]}/mnt/sd${dstarr[$i]}/hg" >> ${dstarr[$i]}.sh
echo "done" >> ${dstarr[$i]}.sh

echo "./"${dstarr[$i]}.sh >> all.sh
(( i++ ))
done

