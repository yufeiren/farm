#! /bin/bash

Bindir=/home/ren/git/farm/tpch/tpch_2_15.0/dbgen
Querydir=$Bindir/queries
Qgen=$Bindir/qgen

scales="1 10 100 500 1000"

cd $Querydir

for scale in $scales
do
	Targetdir=$Bindir/queries-mysql-s$scale

	test -d $Targetdir || mkdir -p $Targetdir

	i=1
	while [ $i -le 22 ]; do
		$Qgen -c -s $scale $i > $Targetdir/mysql-s$scale-q$i.sql
		if [ $i -ne 1 ] && [ $i -ne 3 ] && [ $i -ne 13 ] && [ $i -le 15 ]
		then
			cat $Targetdir/mysql-s$scale-q$i.sql >> $Targetdir/mysql-s$scale-all.sql
		fi
		(( i++ ))
	done
done

