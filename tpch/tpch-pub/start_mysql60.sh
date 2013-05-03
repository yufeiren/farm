FSPATH="/mnt/sdc/mysql"

PARAM="--no-defaults --port=330 \
--socket=/tmp/mysql.sock \
--user=root  \
--datadir=$FSPATH \
--max_connections=3000 \
--max_connect_errors=10 \
--table_cache=2048 \
--max_allowed_packet=1M \
--binlog_cache_size=1M \
--max_heap_table_size=64M \
--sort_buffer_size=64M \
--read_buffer_size=64M \
--join_buffer_size=64M \
--thread_cache=16 \
--thread_concurrency=16 \
--thread_stack=196K \
--query_cache_size=0 \
--ft_min_word_len=4 \
--default_table_type=MYISAM \
--transaction_isolation=REPEATABLE-READ \
--tmp_table_size=64M \
--skip-locking \
--key_buffer_size=26G --tmpdir=/usb/rawdata"

MYSQL_PATH=/mnt/data/vadim/bin/mysql6.0.4
$MYSQL_PATH/libexec/mysqld $PARAM 

