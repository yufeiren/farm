#! /bin/bash

i=0;
j=0;
k=0;
m=0;
n=0;

while [ "$k" != 16 ];
do
	j=0;
	while [ "$j" != 16 ];
	do
		i=0;
		while [ "$i" != 16 ];
		do
			printf "%d %d %d \n" $i $j $k
			i=$(($i+1))
		done
		j=$(($j+1))
	done
	k=$(($k+1))
done

exit;