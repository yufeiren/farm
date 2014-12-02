#! /bin/bash

i=0
while read line           
do
	echo $line
	(( i++ ))
	if [ $i -eq 3 ]; then
		i=0
		echo "0.0"
	fi
done < a.txt

