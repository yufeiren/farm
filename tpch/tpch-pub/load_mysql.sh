PATH_DATA=/usb/rawdata/dbgen.10G
DATABASE=tpch10G

TABLES="part partsupp customer lineitem nation orders region supplier"
#TABLES="nation orders region supplier"
#TABLES="region"

for i in $TABLES
do
echo "Load table $i"
mysql -S /tmp/mysql.sock $DATABASE -e "TRUNCATE TABLE $i"
#mysql -S /tmp/mysql.sock $DATABASE -e "ALTER TABLE $i DISABLE KEYS"
echo `date`
mysql -S /tmp/mysql.sock $DATABASE -e "LOAD DATA INFILE '$PATH_DATA/$i.tbl' INTO TABLE $i FIELDS TERMINATED BY '|'"
#mysql -S /tmp/mysql.sock $DATABASE -e "ALTER TABLE $i ENABLE KEYS"
done

mysql -S /tmp/mysql.sock $DATABASE -e "SHOW TABLE STATUS"
