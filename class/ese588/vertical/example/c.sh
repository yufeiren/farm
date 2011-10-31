#! /bin/bash

x=0
while [ $x -lt 10000 ]; do
        printf "%d 1 2 3 4 5 6 7 8 9 10\n" $x
        (( x++ ))
done
