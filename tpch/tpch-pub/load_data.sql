TRUNCATE TABLE LINEITEM;
TRUNCATE TABLE ORDERS;
TRUNCATE TABLE CUSTOMER;
TRUNCATE TABLE PARTSUPP;
TRUNCATE TABLE SUPPLIER;
TRUNCATE TABLE PART;
TRUNCATE TABLE NATION;
TRUNCATE TABLE REGION;

load data infile '/data/tpch/oneg/region.tbl' into table REGION fields terminated by '|' ;
load data infile '/data/tpch/oneg/nation.tbl' into table NATION fields terminated by '|' ;
load data infile '/data/tpch/oneg/part.tbl' into table PART fields terminated by '|' ;
load data infile '/data/tpch/oneg/supplier.tbl' into table SUPPLIER fields terminated by '|' ;
load data infile '/data/tpch/oneg/partsupp.tbl' into table PARTSUPP fields terminated by '|' ;
load data infile '/data/tpch/oneg/customer.tbl' into table CUSTOMER fields terminated by '|' ;
load data infile '/data/tpch/oneg/orders.tbl' into table ORDERS fields terminated by '|' ;
load data infile '/data/tpch/oneg/lineitem.tbl' into table LINEITEM fields terminated by '|' ;
