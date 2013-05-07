#! /bin/bash

Bindir=/home/ren/git/farm/tpch/tpch_2_15.0/dbgen
Querydir=$Bindir/queries
Qgen=$Bindir/qgen

scales="1 10 100 500 1000"

cd $Querydir

i=1

for scale in $scales
do
	Targetdir=$Bindir/queries-mysql-s$scale

	test -d $Targetdir || mkdir -p $Targetdir

	while [ $i -le 22 ]; do
		$Qgen -c -s $scale $i > $Targetdir/mysql-s$scale-q$i.sql
		(( i++ ))
	done
done

