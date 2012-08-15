use nmon

delete from nmon_zzzz;
delete from nmon_top;
delete from testcase;

LOAD DATA LOCAL INFILE 'example-zzzz.log'
INTO TABLE nmon.nmon_zzzz
FIELDS TERMINATED BY ',' ENCLOSED BY '"'
LINES TERMINATED BY '\n'
(zzzz_tag, zzzz_date);

LOAD DATA LOCAL INFILE 'example-top.log'
INTO TABLE nmon.nmon_top
FIELDS TERMINATED BY ',' ENCLOSED BY '"'
LINES TERMINATED BY '\n'
(top_tag, top_cpu);

LOAD DATA LOCAL INFILE 'testcase-date-start-end.log'
INTO TABLE nmon.testcase
FIELDS TERMINATED BY ',' ENCLOSED BY '"'
LINES TERMINATED BY '\n'
(start, end);

UPDATE nmon_top, nmon_zzzz
 SET top_date = zzzz_date
 WHERE top_tag = zzzz_tag;
 
UPDATE testcase SET cpu = (select avg(nmon_top.top_cpu)
 from nmon_top where testcase.start < nmon_top.top_date
      and nmon_top.top_date < testcase.end );

SELECT cpu FROM testcase ORDER BY start;
