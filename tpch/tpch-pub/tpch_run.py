#!/usr/bin/env python

"""
tpch audit run

Options:
  -d             debug mode
  -h, --help     show this help
  -q, --query_program=  program for query streams, default to use mysql
  -r, --root=    root directoy
  -s, --scale=   scale factor (default 1, 1GB)
  -p, --power    power test only
  -t, --throughput  throughput test only
  -u, --update   do update or rf1, rf2
  --test_only     only test
  --db=          database name
  
"""

__author__ = "David Zhang"
__version__ = "$Revision: 1.0 $"
__date__ = "$Date: 2007/08/08 $"

import sys
import getopt
import re
import random
import copy
from threading import Thread
import difflib
import os
from sets import Set
import pdb
import time
import signal
import shutil
import inspect
import MySQLdb
import math

# global variables
tpch_db_name = "tpch100G"

_debug = 1
mysock="/tmp/mysql.sock"
env_view_root=""
mysql_test_dir=""
mysql_cmd = "cat %s | /mnt/data/vadim/bin/mysql5.1.13/bin/mysql -S /tmp/mysql.sock "+tpch_db_name+" --force -u root -v -v -v"
mysql_query = "/mnt/data/vadim/bin/mysql5.1.13/bin/mysql -S /tmp/mysql.sock "+tpch_db_name+" --force -u root -Bse '%s'"
mysql_cmd_loader = ""
load_cmd=""


# global variables
script_log_file=""
run_id=7
scale_factor=1
rand_seed=8768640832410
#rand_seed=1
audit_dir=""
dbgen_dir=""
qgen=""
output_dir=""
data_dir=""
num_streams_scale ={1:2,10:3,30:4,100:5,300:6,1000:7,3000:8,10000:9}
loader_input_dir={1:'/nfs/dumpster/tpch_fixed_width/1G_col_order',10:'/nfs/dumpster/tpch_fixed_width/10G_col_order',30:'',100:'/nfs/dumpster/tpch_fixed_width/100G_col_order',300:''}
#loader_input_dir={1:'/data/dzhang/FL_1G_data',10:'/data/dzhang/FL_10G_col_order',30:'',100:'/data/dzhang/FL_100G_col_order',300:''}
#inc_loader_input_dir={1:'/nfs/dumpster/tpch_fixed_width/inc_1G_data',10:'/nfs/dumpster/tpch_fixed_width/inc_10G_data',30:'',100:'/nfs/dumpster/tpch_fixed_width/inc_100G_data',300:''}
inc_loader_input_dir={1:'/data/dzhang/IL_1G_input',10:'/nfs/dumpster/tpch_fixed_width/inc_10G_data',30:'',100:'/nfs/dumpster/tpch_fixed_width/inc_100G_data',300:''}
gen_new_rfs = True             # if set, gen RF data using dbgen on the fly

do_power=False
do_throughput=False
do_update=False
do_loader=False
rf_tmp_file_name_template = "%s/rf%s_%s_s%s.sql" 
query_file_template="%s/query_%s_s%s.sql"
use_mysql_client = False
use_mysql_client = True
tpch_conn = None
test_only=False
rf_throughput_concurrent=False    # if true start RF and throughput concurrently, or start throughput first then RF, concurrent may be better as query will not use CPU a lot
                                  # this will cause deadlock
num_seqs=1                        # how many iterations
num_rf_streames=1                 
#tpch_db_namse = "tpcd"
#tpch_db_name = "tpcd_1G"
update_only = False              # only do update
no_cleanup = False               # do not cleanup 
leave_server_up = False          # leave the server up after the run
gen_udf_call = False              # true if to use UDF instead of profiling

qtime=[]   # query time (seq, stream_id, qnum, start,end, time)
rftime=[]  # refresh time (seq, stream_id, rf_id, start, end, time)
streams_time=[]  # streams time  (seq, stream_id, start, end, duration)
tpch_power_numbers=[]
tpch_throughput_numbers=[]
all_queries={}

# for reporting
harp_run_time_log=""
master_err_file_hanle=None
master_err_file=None

#====Utilties============

class TimedOutExc(Exception):
    def __init__(self, value = "Timed Out"):
        self.value = value
    def __str__(self):
        return repr(self.value)



def sys_call(cmd):
#    dbg_print(os.getcwd())
#    print (cmd)
    dbg_print("%s:%s" % (os.getcwd(),cmd))
    os.system(cmd)

def sys_pipe_call(cmd):
    dbg_print("%s:%s" % (os.getcwd(),cmd))
    return os.popen(cmd).read()               
#    return os.popen("echo " + cmd).read() 
#    return cmd

def sys_pipe_call_1(cmd):
    ''' also return the errors '''
#    time.sleep(0.1)
#    return cmd
    dbg_print("%s:%s" % (os.getcwd(),cmd))
    return os.popen4(cmd)[1].read()
    #return cmd

def sys_call_env(cmd):
    cmds=cmd.split()
    dbg_print("cmds= %s " % cmds)
    os.spawnv( os.P_WAIT, cmds[0], cmds[1:])

def usage():
    caller_name = sys._getframe(1).f_code.co_name
    #print "caller_name = %s " % caller_name
    if (caller_name=='main'):
        print __doc__
    else:
        print globals()[caller_name].__doc__
    sys.exit(0)                  

def dbg_print(in_str):
    if _debug:
        print ("== " + sys._getframe(1).f_code.co_name + " == " + str(in_str))

def whoami():
    return sys._getframe(1).f_code.co_name

# tpch_
def tpch_error(s):
        print ("TPCH Error:" + s)
        sys.exit(1)
        
def get_time():
    return time.time()

def write_log(s,log_file_name=None,timestamp=True,pstdout=False):
    if not log_file_name: pstdout = True  # print the master
    indent_start=4
    if not log_file_name: log_file_name = script_log_file
    #dbg_print("log_file_name=%s" % log_file_name)
    #dbg_print("script_log_file=%s" % script_log_file)
    #indent = "".join(["  " for i in range(len(inspect.stack()))])
    indent = "".rjust((len(inspect.stack())-indent_start)*3)
    ts = timestamp and "%s :" % time.strftime('%x %X') or ""
    line = ts + indent + s + "\n"
    open(log_file_name,"a+").write(line)
    if pstdout: sys.stdout.write(line)
    if not log_file_name: dbg_print(line)

def setup_mysql_udfs():
    udfs = '''
    # create the UDF
drop function if exists c2loader;
create function c2loader returns string soname "c2loader.so";
'''
    in_file = "%s/mysql_init.sql" %  (output_dir)
    open(in_file,"w").write(udfs)
    output = sys_pipe_call_1(mysql_cmd % in_file)
    out_file = "%s/mysql_init.log" %  (output_dir)
    open(out_file,"w").write(output)

import distutils.dir_util
def setup_mysql_copy_seed(force=False):
    db_dir = "%s/var/master-data" % mysql_test_dir
    if (force):
        write_log("Remove database and copy seed",pstdout=True)
        import shutil
        shutil.rmtree(db_dir)
        os.mkdir(db_dir)
    if not os.path.isfile("%s/data.c2db" % db_dir):
        distutils.dir_util.mkpath(db_dir)
        sys_call("cp -rf  %s/std_data/c2db_seed_db/* %s" % (mysql_test_dir, db_dir))
    distutils.dir_util.mkpath("%s/var/log" % mysql_test_dir)
def setup_mysql():
    global mysql_test_dir, mysock, env_view_root
    mysql_test_dir=os.getenv('MYSQL_TEST_DIR')
    mysock=os.getenv('MASTER_MYSOCK')
    env_view_root=os.getenv('VIEW_ROOT')
    if not mysql_test_dir:
        if (env_view_root):
            mysql_test_dir="%s/mysql-5.1/mysql-test" % env_view_root
            #mysock='%s/mysql-5.1/mysql-test/var/tmp/master.sock' % env_view_root
            mysock='%s/master.sock' % audit_dir
    if not mysql_test_dir: raise AssertionError, "MYSQL_TEST_DIR is not defined\n"
    setup_mysql_copy_seed(do_loader and not no_cleanup)  # cleanup

    os.chdir("%s/var/log" % mysql_test_dir)
    if not mysock: raise AssertionError, "no Master Sock\n"
    global mysql_cmd, mysql_cmd_loader
    mysql_cmd = " cat %s | mysql --sock=" + mysock + " --database=%s --force  -u root -v -v -v" % tpch_db_name # disregard error
    mysql_cmd_loader = " cat %s | mysql --sock=" + mysock + " --force  -u root -v -v -v" 
    dbg_print("mysql_cmd = %s" % mysql_cmd)
    #if server_is_up(): shutdown_server()
    start_up_server_if_needed()
    #setup_mysql_udfs()
    
def setup_tpch():
    global audit_dir, dbgen_dir, qgen, output_dir, script_log_file, harp_run_time_log
    audit_dir=os.getenv('AUDIT_DIR')
    global env_view_root
    env_view_root=os.getenv('VIEW_ROOT')
    mysql_test_dir=os.getenv('MYSQL_TEST_DIR')
    if not audit_dir:
        if (env_view_root):
            audit_dir="%s/c2-test/tpch_audit" % env_view_root
        if (mysql_test_dir):
            audit_dir="%s/../../c2-test/tpch_audit" % mysql_test_dir
    if not audit_dir: raise AssertionError, "AUDIT_DIR is not defined and not in a view\n"

    if not dbgen_dir: dbgen_dir="%s/dbgen" % audit_dir
    if not os.path.isfile("%s/dbgen" % dbgen_dir):  # compile if needed
        try:
            saved_cur_dir = os.getcwd()
        except OSError:
            saved_cur_dir=audit_dir
        os.chdir(dbgen_dir)
        os.environ['PATH'] = ".:%s" %os.environ['PATH']
        sys_call("make -f makefile")
        os.chdir(saved_cur_dir)
        if not os.path.isfile("%s/dbgen" % dbgen_dir):
            tpch_error("%s/dbgen does not exist" % dbgen_dir)

    try:
        os.mkdir("%s/results" % audit_dir)
    except: pass
    output_dir = "%s/results/r%s" % (audit_dir, run_id)
    try:
        shutil.rmtree(output_dir)
    except: pass
    os.mkdir(output_dir)

    qgen = "%s/qgen" % dbgen_dir
    # set this so qgen can find the query template
    os.environ['DSS_QUERY'] = '%s/queries' % (dbgen_dir)
    os.environ['DSS_PATH'] = output_dir    # this is where the update file is stored
    os.environ['DSS_CONFIG'] = dbgen_dir
    
    script_log_file = "%s/tpch.log" % (output_dir)
    harp_run_time_log = "%s/harp_run_time.log" % (output_dir)

    # setup_mysql()   # setup mysql related

def gen_load_tpch_file(in_file):
    input_dir = loader_input_dir[scale_factor]
    sys_call("sed 's/loader_databasename/%s/g' %s/include/c2loader_table_creation_with_index_with_alterudf.inc > %s/include/c2loader_table_creation_with_index_with_alterudf.inc.tmp" % (tpch_db_name,mysql_test_dir,mysql_test_dir ))
    if not input_dir or not os.path.isdir(input_dir): tpch_error("Input directory '%s' does not exist" % input_dir)
    if gen_udf_call:
        udf_call = "source %s/include/c2loader_table_creation_with_index_with_alterudf.inc.tmp;" % mysql_test_dir or ""
        loader_options = ",'-flags', 'NoInputProfiling=yes'"
    else:
        udf_call = ""
        loader_options = ",'-flags', 'AutoIndex=yes'"
        #loader_options = ""     # enable this afeter AutoIndex is default
    cmd = '''
    drop database if exists %s;
    create database %s;
    use %s;
    
    drop function if exists c2loader;
    create function c2loader returns string soname "c2loader.so";

    set @cur_db='%s';
    set storage_engine=c2db;
    source %s/scripts/c2loader_table_creation_with_index.inc;
    use %s;
    show tables;
    %s

    select c2loader('INITIAL',
        '%s.region', 
        '%s/region.tbl', 
        'FIXED_WIDTH_ROW',
        'r_name:25#:r_comment:116#',

        '%s.nation', 
        '%s/nation.tbl', 
        'FIXED_WIDTH_ROW',
        'n_name:25#:n_comment:116#',

        '%s.customer', 
        '%s/customer.tbl', 
        'FIXED_WIDTH_ROW',
        'c_name:25#:c_address:40#:c_phone:15#:c_mktsegment:10#:c_comment:117#',

        '%s.orders', 
        '%s/orders.tbl', 
        'FIXED_WIDTH_ROW',
        'o_orderstatus:1#:o_orderpriority:15#:o_clerk:15#:o_comment:79#',

        '%s.part', 
        '%s/part.tbl', 
        'FIXED_WIDTH_ROW',
        'p_name:55#:p_mfgr:25#:p_brand:10#:p_type:25#:p_container:10#:p_comment:23#',

        '%s.supplier', 
        '%s/supplier.tbl', 
        'FIXED_WIDTH_ROW',
        's_name:25#:s_address:40#:s_phone:15#:s_comment:101#',

        '%s.partsupp', 
        '%s/partsupp.tbl', 
        'FIXED_WIDTH_ROW',
        'ps_comment:199#',

        '%s.lineitem', 
        '%s/lineitem.tbl', 
        'FIXED_WIDTH_ROW',
        'l_quantity:011.00:l_returnflag:1#:l_linestatus:1#:l_shipinstruct:25#:l_shipmode:10#:l_comment:44#'

        %s
        );

    ''' % (tpch_db_name, tpch_db_name, tpch_db_name, tpch_db_name, audit_dir, tpch_db_name,
    udf_call,                  # gen udf call
    tpch_db_name, input_dir,   # region
    tpch_db_name, input_dir,   # nation
    tpch_db_name, input_dir,   # customer
    tpch_db_name, input_dir,   # orders
    tpch_db_name, input_dir,   # part
    tpch_db_name, input_dir,   # supplier
    tpch_db_name, input_dir,   # partsupp
    tpch_db_name, input_dir, loader_options)   # lineitem, avoid confusing emacs, add '
    open(in_file,"w").write(cmd)

def load_tpch():
    ''' db loading, for now, just copy saved db from qadumpster to master-data
        this will also verify
    '''
    line_header="Loading"
    write_log("Start: load database")
    # generation
    in_file = "%s/loader.sql" %  (output_dir)
    cnt_file = "%s/cnt.sql" % (output_dir)
    cnt_cmd='''
use %s
select count(*) from region;
select count(*) from nation; 
select count(*) from customer;
select count(*) from orders;
select count(*) from supplier;
select count(*) from part;
select count(*) from partsupp;
select count(*) from lineitem;

    ''' % tpch_db_name
    open(cnt_file,"w+").write(cnt_cmd)
    cnt_log = "%s/cnt.log" % (output_dir)
    gen_load_tpch_file(in_file)
    if (do_loader):
        # run
        local_log = "%s/loader.log" %  (output_dir)
        write_log("%s begin" % (line_header))
        write_log("%s begin" % (line_header), local_log)
        start_time=get_time()
        if not test_only: output = sys_pipe_call_1(mysql_cmd_loader % in_file)
        else: output = "test only"
        end_time = get_time()
        duration = end_time - start_time
        write_log("%s start time %s" % (line_header, conv_to_date(start_time)), local_log)
        open(local_log,"a+").write(output)
        write_log("%s end time %s" % (line_header, start_time))
        write_log("%s end time %s" % (line_header, start_time), local_log)
        write_log("%s end time %s" % (line_header, end_time))
        write_log("%s end time %s" % (line_header, end_time), local_log)
        write_log("%s duration %s" % (line_header, duration))
        write_log("%s duration %s" % (line_header, duration), local_log)
        global rand_seed
        if rand_seed >=0:  
            rand_seed = long("".join(["%02d" % x for x in time.localtime(end_time)[1:6]]))
            write_log("Seed based on loading end time = %s" % rand_seed)
        else:
            rand_seed = None
    write_log("End: load database")
    if not test_only:
        output = sys_pipe_call_1(mysql_cmd_loader % cnt_file)
        open(cnt_log,"w+").write(output)
#    raw_input("End of loading #1 . Press any key to continue!!!")
#    raw_input("End of loading #2. Press any key to continue!!!")
#    raw_input("End of loading #3. Press any key to continue!!!")
    
    # restart here
#    write_log("Restart MySQL server after loading", pstdout=True)
#    if server_is_up(): shutdown_server()
#    start_up_server_if_needed()

def compute_result(power_run,seq,num_streams,duration,result):
    if power_run:
        run_time = [x[-1] for x in qtime if x[0] == seq and x[1] == 0 ]
        run_time.extend([x[-1] for x in rftime if x[0] == seq and x[1] == 0 ])
    else:
        run_time = [x[-1] for x in qtime if x[0] == seq and x[1] != 0 ]
        run_time.extend([x[-1] for x in rftime if x[0] == seq and x[1] != 0 ])
        
    num_queries_and_rfs = len(run_time)
    write_log("%s Seq %s num queries and rfs = %s" % (power_run and "Power" or "Throughput", seq,num_queries_and_rfs), pstdout=True)
    geo_mean = math.exp(sum([math.log(t) for t in run_time])*1.0/float(num_queries_and_rfs))
    arith_mean = sum([t for t in run_time])/float(num_queries_and_rfs)
    if power_run: result_number = 3600.0* geo_mean *scale_factor
    else: result_number = (num_streams*22.0*3600)/duration*scale_factor
    result.append((result_number, geo_mean, arith_mean))

def print_results():
    ''' print all the results '''
    qtime.sort(lambda x,y: int(float(x[0]*10000+x[1]*100+x[2])-float(y[0]*10000+y[1]*100+y[2])))
    
    write_log("TPCH Detailed Results")
    for i in range(num_seqs):   # seq number
        tmp_qtime = [list(x) for x in qtime if x[0]==i+1]
        for q in tmp_qtime:
            write_log("Seq %s, Streams %s, Query %s: start_time= %s, end_time = %s, duration = %s " % (tuple(q)))
        tmp_qtime=([list(x) for x in rftime if x[0] ==i+1])
        for q in tmp_qtime:
            write_log("Seq %s, Streams %s, RF %s: start_time = %s, end_time=%s, duration = %s " % (tuple(q)))

    write_log("TPCH Streams Results")
    streams_time.sort(lambda x,y: int(float(x[0]*100+x[1])-float(y[0]*100+y[1])))
    for x in streams_time:
#        write_log("Seq %s, Streams %s: start_time = %s , end_time = %s, duration = %s " % x)
        (seq_id, stream_id, start_time, end_time, duration) = x
        stream_rand_seed = rand_seed and long(rand_seed) + stream_id or 'default'
        if stream_id<100: tmp_x = [seq_id, "Query Streams %s" % stream_id, "rand_seed = %s" % stream_rand_seed]  
        else: tmp_x = [seq_id, 'RF throughput streams','']   # RF is 1001 for sorting purpose
        tmp_x.append(conv_to_date(start_time))
        tmp_x.append(conv_to_date(end_time))
        tmp_x.append(duration)
        write_log("Seq %s, %s: %s, start_time = %s , end_time = %s, duration = %s " % tuple(tmp_x))
        
    # aggregated results
    if do_power:
        write_log("TPCH Power Tests Results", pstdout=True)
        for i in range(num_seqs):
            write_log("  Power  Seq %s: (Power Metric, GeoMean, ArithMean) = %s  " % (i+1, tpch_power_numbers[i]), pstdout=True)
    if do_throughput:
        write_log("TPCH Throughput Tests Results",pstdout=True)
        for i in range(num_seqs):
            write_log("  ThroughPut Seq %s: (Throughput Metric, GeoMean, ArithMean) = %s " % (i+1, tpch_throughput_numbers[i]), pstdout=True)
    if do_power and do_throughput:
        write_log("TPCH QPH Results",pstdout=True)
        for i in range(num_seqs):
            qph = math.exp((math.log(tpch_power_numbers[i][0]) + math.log(tpch_throughput_numbers[i][0]))*0.5)
            write_log("  TPCH QPH for Seq %s  = %s " % (i+1, qph), pstdout=True)
        
    if server_is_up():
        if leave_server_up: write_log("  Leave MySQL server up", pstdout=True)
        else: shutdown_server()
    else:
        write_log("  MySQL Server is down during the Run", pstdout=True)
        sys.exit(9);
            
def run_tpch_all():
    #load_tpch()   # load the data
    write_log("Start TPC-H Benchmark for Run id: %s " % run_id)
    if do_update:
        gen_rfs()
    for i in range(num_seqs):
        if do_power: run_tpch_power_test(i+1)
        if do_throughput: run_tpch_throughput_test(i+1)
        #start_up_server_if_needed()   # restart server
    print_results()
    write_log("End TPC-H Benchmark for Run id: %s " % run_id)
#    pdb.set_trace()

def parse_query_file(query_file):
    lines = [l.rstrip() for l in open(query_file,"r").readlines()]
    #dbg_print("lines = %s " % lines)
    query_begin_re = re.compile("TPC-H/TPC-R .* Query \(Q([0-9]+)\)")
    #query_end_re = re.compile(";$")
    query_end_re = re.compile("^set rowcount ([\-0-9]+)")
    dbg_print("query_file = %s " % query_file)
    first_l=lines[0]
    query_begin = False
    queries=[]
    for l in lines:
        #dbg_print("l= %s" % l)
        m = query_begin_re.search(l)
        if m:
            #dbg_print("begin l= %s " % l)
            query_begin = True
            cur_qry=[]
            qry_name=m.group(1)
            cur_qry.append(first_l)                  
        if query_begin:
            m1 = query_end_re.search(l)
            if m1:
                query_begin = False
                row_count = int(m1.group(1))
                if row_count > 0:
                    last_line = cur_qry[-1]
                    cur_qry[-1] =  last_line.split(";")[0] + " limit %s;" % row_count
                queries.append((int(qry_name),"\n".join(cur_qry)))
            else:  cur_qry.append(l)

    #dbg_print("queries = %s " % queries)
    if len(queries) != 22:
        print "queries = %s " % queries
        raise AssertionError, "There should be 22 queries"
    return queries

def conv_to_date(t):
    return time.strftime("%X %x",time.localtime(t))

def open_connection():
    if (use_mysql_client): return None
    global tpch_conn
    tpch_conn = MySQLdb.connect (host="localhost", user="root", passwd="",db=tpch_db_name,unix_socket=mysock)
    if not tpch_conn:  raise AssertionError, "Connection failure"
    else: return tpch_conn
    
def close_connection():
    if (use_mysql_client): return
    tpch_conn.close()

def execute_one_query(seq, stream_id, qnum, q):
    # to test rf, do not run any query     e they do not pass

    def handler(signum, frame):
        raise TimedOutExc()

    if (test_only or update_only):
        time.sleep(0.1)
        return "Fake results\n"
    out_file = "%s/query_%s_s%s_q%s.log" % (output_dir,seq,stream_id,qnum)
    if (use_mysql_client):
	timeout = 3600
        in_file = "%s/query_%s_s%s_q%s.txt" % (output_dir,seq,stream_id,qnum)
        open(in_file,"w").write(q)
	old = signal.signal(signal.SIGALRM, handler)
        signal.alarm(timeout)
	try:
		try:
        		output = sys_pipe_call_1(mysql_cmd % in_file)
		except TimedOutExc:
			print "Too long"
			output = "NA"
			#kill connection
			outkill = sys_pipe_call(mysql_query % "SHOW PROCESSLIST")
			lines = outkill.split('\n')
        		for l in lines:
				par = l.split('\t')
				if len(par)>=6:
					print "Query time %s" % par[5]
					if int(par[5]) >= timeout : 
						print "Line %s time %s" % ( par[0], par[5])
						outkill = sys_pipe_call(mysql_query % ( "KILL %s" % par[0] ) )
	finally:
		signal.signal(signal.SIGALRM, old)
        #os.remove(in_file)
        open(out_file,"w").write(output)
    else:
        cursor = tpch_conn.cursor()   # may need to start timer here
        #cursor.execute("SELECT * from mysql.c2v$_tables") # this is just a test
	
	old = signal.signal(signal.SIGALRM, handler)
        signal.alarm(5)
	#try:
	try:
       		cursor.execute(q)    # uncomment this for real run
	finally:
		signal.signal(signal.SIGALRM, old)

       	result = cursor.fetchall()
       	cursor.close()
	#except TimedOutExc:
	#	print "Too long"

	signal.alarm(0)

        output = "\n".join(["|".join([str(f) for f in r]) for r in result])
        #dbg_print("output = %s" % output)
        open(out_file,"w").write(output)
    return output

def run_query(seq, stream_id):
    ''' this is a wrapper. For now use mysql client. Later on can be a C client for query execution. Need to time each queries
        For now, satisfy the requirement to run all queries in a single session. Will try to use python connection
        may need to disable logging for throughput run
    '''
    local_log = "%s/query_%s_s%s.log" % (output_dir, seq, stream_id)
    line_header="Query Stream"
    queries=all_queries["%s_%s" % (seq,stream_id)]
    global qtime
    open_connection()
    for (qnum, q) in queries:
        write_log("%s %s seq %s qnum %s begin" % (line_header, stream_id, seq, qnum))
        write_log("%s %s seq %s qnum %s begin" % (line_header, stream_id, seq, qnum), local_log)
        start_time=get_time()
        output = execute_one_query(seq, stream_id, qnum, q)
        end_time = get_time()
        duration = end_time - start_time
        qtime.append((seq, stream_id, qnum, start_time, end_time, duration))
        write_log("%s seq %s stream %s qnum %s start time %s" % (line_header, seq, stream_id, qnum, start_time))
        write_log("%s seq %s stream %s qnum %s start time %s" % (line_header, seq, stream_id, qnum, start_time), local_log)
        open(local_log,"a+").write(output)
        write_log("%s seq %s stream %s qnum %s end time %s" % (line_header, seq, stream_id, qnum, end_time))
        write_log("%s seq %s stream %s qnum %s end time %s" % (line_header, seq, stream_id, qnum, end_time), local_log)
        write_log("%s seq %s stream %s qnum %s duration %s" % (line_header, seq, stream_id, qnum, duration))
        write_log("%s seq %s stream %s qnum %s duration %s" % (line_header, seq, stream_id, qnum, duration), local_log)
    close_connection()
    
def gen_rfs():
    ''' generate rf1, rf2 using dbgen. eventually in a temp table after loading '''
    if not do_update: return
    num_streams = num_streams_scale[scale_factor]
    if do_power and not do_throughput:
        num_rf_streams = 1
    elif not do_power and do_throughput:
        num_rf_streams = num_streams 
    elif do_power and do_throughput:
        num_rf_streams = num_streams + 1
    dbg_print("gen_rfs begin, num_rf_streams=%s"%num_rf_streams )
    try:
        saved_cur_dir = os.getcwd()
    except OSError:
        saved_cur_dir=audit_dir
    os.chdir(output_dir)
    if gen_new_rfs:
        input_dir = output_dir
        if not test_only:
            sys_call("%s/dbgen -f -O m -s %s -U %s" % (dbgen_dir, scale_factor, num_rf_streams*num_seqs))
        else:
            print("%s/dbgen -f -O m -s %s -U %s" % (dbgen_dir, scale_factor, num_rf_streams*num_seqs))
    else:
        input_dir = inc_loader_input_dir[scale_factor]
    rf_files=["orders.tbl.u%s","lineitem.tbl.u%s","delete.%s"]
    for seq in range(num_seqs):
        for stream_id in range(num_rf_streams):
            rf_seq_no = (stream_id + 1 + num_rf_streams * seq)
            for f in [x % rf_seq_no for x in rf_files]:
                f_full = "%s/%s" % (input_dir,f)
                dbg_print("inc input file = %s" % f_full)
                if not test_only:
                    if not os.path.isfile(f_full): tpch_error("RF file %s does not exist" % f_full)
                    sys_call("ln -s %s rf_%s_%s_%s" % (f_full, seq+1, stream_id, f.split(".")[0]))
    dbg_print("gen_rfs done")
    os.chdir(saved_cur_dir)
    for seq in range(num_seqs):
        for stream_id in range(num_rf_streams):
            for i in range(2):  # RF1, RF2
                gen_tmp_rf_file(i+1, seq+1, stream_id)

def gen_tmp_rf_file(rf_id, seq, stream_id):
    ''' generate the sql from the dbgen update file. This is temporary and may not be used when temp tables are use '''
    #dbgen_update_files = (['orders.tbl.u', 'lineitem.tbl.u'], ['delete.'])
    file_name = rf_tmp_file_name_template % (output_dir, rf_id, seq,stream_id)
    #update_files=["%s/%s%s" % (dbgen_dir, x, stream_id) for x in dbgen_update_files[rf_id-1]]
    dbg_print("in gen_tmp_rf_file rf_id= %s " % rf_id)
    if rf_id==1:  # update
        orders_file="%s/rf_%s_%s_%s" % (output_dir,  seq, stream_id, 'orders')
        lineitem_file="%s/rf_%s_%s_%s" % (output_dir, seq,stream_id, 'lineitem')
        f = open(file_name,"w")
        cmd = '''
        drop function if exists c2loader;
        create function c2loader returns string soname "c2loader.so";
        
        select c2loader('INCREMENTAL', 'INSERT', 'YES',
                '%s.orders', 
                '%s', 
                'FIXED_WIDTH_ROW',
                'o_orderstatus:1#:o_orderpriority:15#:o_clerk:15#:o_comment:79#',

                '%s.lineitem', 
                '%s', 
                'FIXED_WIDTH_ROW',
                'l_quantity:011.00:l_returnflag:1#:l_linestatus:1#:l_shipinstruct:25#:l_shipmode:10#:l_comment:44#');
        ''' % (tpch_db_name,orders_file, tpch_db_name,lineitem_file)   # avoid confusing emacs, add '

        f.write('set SQL_LOG_BIN=0;\n')  # disable the bin log
        
        f.write(cmd)
        f.close()
    elif rf_id==2:  # delete
        orders_delete_file =  "%s/rf_%s_%s_delete" % (output_dir, seq, stream_id)
        f = open(file_name,"w")
        cmd='''
        select c2loader('INCREMENTAL', 'DELETE', 'YES',
                '%s.orders', 
                '%s', 
                'FIXED_WIDTH_ROW',
                '');
        ''' % (tpch_db_name,orders_delete_file)      # avoid confusing emacs, add '
        f.write('set SQL_LOG_BIN=0;\n')  # disable the bin log
        f.write(cmd)
        f.close()
    
def run_rf(rf_id, seq, stream_id):
    ''' oracle load the update in a temp table and do insert as select. For now, just get the file and run it.
        insert into orders, lineitems (o_orderkey) '''
    global rftime
    if test_only:
        rftime.append((seq, stream_id, rf_id, 1, 2, 3))
    if not do_update:
        #rftime.append((seq, stream_id, rf_id, 101, 10))  # fake it
        return
    dbg_print("begin run_rf %s " % rf_id)
    tmp_file = rf_tmp_file_name_template % (output_dir, rf_id, seq,stream_id)
    local_log = "%s/rf%s_%s_s%s.log" % (output_dir, rf_id, seq, stream_id)
    line_header="RF %s" % rf_id
    write_log("%s seq %s stream %s begin" % (line_header, seq, stream_id))
    write_log("%s seq %s stream %s begin" % (line_header, seq, stream_id), local_log)
    start_time=get_time()
    if not test_only: output = sys_pipe_call_1(mysql_cmd % tmp_file)
    else: output = "test only"
    #os.remove(tmp_file)
    end_time = get_time()
    duration = end_time - start_time
    write_log("%s seq %s stream %s start time %s" % (line_header, seq, stream_id, start_time), local_log)
    write_log("%s seq %s stream %s start time %s" % (line_header, seq, stream_id, start_time))
    # no need to print out result for rf, 
    open(local_log,"a+").write(output)
    write_log("%s seq %s stream %s end time %s" % (line_header, seq, stream_id, end_time))
    write_log("%s seq %s stream %s end time %s" % (line_header, seq, stream_id, end_time), local_log)
    write_log("%s seq %s stream %s duration %s" % (line_header, seq, stream_id, duration))
    write_log("%s seq %s stream %s duration %s" % (line_header, seq, stream_id, duration), local_log)
    rftime.append((seq, stream_id, rf_id, start_time, end_time, duration))
    
def run_qgen(stream_id, seq):
    query_parameter_file="%s/qp_%s_s%s.log" % (output_dir, seq, stream_id)
    query_file= query_file_template % (output_dir, seq, stream_id)
    dbg_print("run_qgen begin")
    try:
        saved_cur_dir = os.getcwd()
    except OSError:
        saved_cur_dir=audit_dir
    os.chdir(dbgen_dir)
    if rand_seed:
        stream_rand_seed = long(rand_seed) + stream_id
        write_log("Generates query template file for streams %s with seed: %s " % (stream_id, stream_rand_seed))
        sys_call("%s -c -r %s -p %s -s %s -l %s > %s" % (qgen, stream_rand_seed, stream_id, scale_factor, query_parameter_file, query_file))
    else:
        write_log("Generates query template file for streams %s with seed: %s " % (stream_id, 'default'))
        sys_call("%s -c -d -p %s -s %s -l %s > %s" % (qgen, stream_id, scale_factor, query_parameter_file, query_file))
    line_header="Query Stream"
    queries = parse_query_file(query_file)
    global all_queries
    all_queries["%s_%s" % (seq,stream_id)] = queries
    # result files are in dbgen_dir/orders.tbl.u1 lineitem.tbl.u1. delete.1
    os.chdir(saved_cur_dir)
    dbg_print("run_qgen end")

def run_tpch_power_test(seq=1):
    stream_id = 0   # power_test is stream 0
    global streams_time
    write_log("Start TPC-H Power test Run id: %s Seq: %s" % (run_id, seq))
    run_qgen(stream_id, seq) 
    stream_start_time =get_time()
    run_rf(1,seq, stream_id)
    run_query(seq, stream_id)
    run_rf(2, seq, stream_id)
    stream_end_time = get_time()
    stream_duration = stream_end_time - stream_start_time
    streams_time.append((seq, stream_id, stream_start_time, stream_end_time, stream_duration))
    write_log("End TPC-H Power test Run id: %s Seq: %s" % (run_id, seq))
    #dbg_print("qtime = %s " % qtime)
    global tpch_power_numbers
    compute_result(True, seq, 1, 0, tpch_power_numbers)

class throughput_stream(Thread):
    (rp, wp) = os.pipe()
    def __init__ (self, seq, stream_id, stream_type='query'):
        Thread.__init__(self)
        self.seq = seq
        self.stream_id = stream_id
        self.stream_type=stream_type
    def run(self):
        global streams_time
        if self.stream_type == 'query':
            stream_start_time =get_time()
            run_query(self.seq, self.stream_id)
            stream_end_time = get_time()
            stream_duration = stream_end_time - stream_start_time
            streams_time.append((self.seq, self.stream_id, stream_start_time, stream_end_time, stream_duration))
            os.write(self.wp,"1")
        else:
            for i in range(num_streams_scale[scale_factor]):
                os.read(self.rp,1)
            write_log("RF unblocking")
            stream_start_time =get_time()
            for i in range(num_streams_scale[scale_factor]):
                run_rf(1,self.seq, i+1)
                run_rf(2, self.seq, i+1)
            stream_end_time = get_time()
            stream_duration = stream_end_time - stream_start_time
            streams_time.append((self.seq, self.stream_id, stream_start_time, stream_end_time, stream_duration))

def run_tpch_throughput_test(seq=1):
    ''' number of query streams run concurrently with one refresh stream. sequential run rf1, rf2 for all the streams
        Oracle waited all the query streams to finish before start rf.
        But MS start rf same time as query streams'''
    write_log("Start TPC-H Through Put test Run id: %s Seq: %s" % (run_id, seq))
    num_streams = num_streams_scale[scale_factor]
    t_streams=[]
    for s in range(num_streams):
        run_qgen(s+1, seq)
        t_streams.append(throughput_stream(seq, s+1, 'query'))
    #for s in range(num_streams):
    t_streams.append(throughput_stream(seq, 1001, 'rf'))
    #c = raw_input("continue? (Y/N)").upper()
    throughput_start_time=get_time()
    if (rf_throughput_concurrent):
        for t in t_streams: t.start()    
        for t in t_streams: t.join()
    else:
        global streams_time
        stream_start_time =get_time()
        for t in t_streams: t.start()
        for t in t_streams: t.join()
        
        #for t in t_streams[0:num_streams]: t.start()   # start the query first
        #for t in t_streams[0:num_streams]: t.join()
        #for s in range(num_streams):
            #t_streams[num_streams + s].start()   # start the rf last
            #t_streams[num_streams + s].join()
        #stream_end_time = get_time()
        #stream_duration = stream_end_time - stream_start_time
        #streams_time.append((seq, 1001, stream_start_time, stream_end_time, stream_duration))
    throughput_end_time=get_time()
    throughput_duration = throughput_end_time - throughput_start_time
    global tpch_throughput_numbers
    compute_result(False, seq, num_streams, throughput_duration,tpch_throughput_numbers)
    write_log("End TPC-H Through Put test Run id: %s, Seq: %s, Scale: %s, Number of Streams: %s" % (run_id, seq, scale_factor, num_streams))
    write_log("Through Put duration  = %s" % (throughput_duration)) 
    
#---------------------------
def server_is_up():
    ret = sys_pipe_call_1("echo exit | mysql --sock=%s" % mysock)
    #print "ret = %s " % ret
#    return True
    return not re.compile("Can't connect to local MySQL server through socket").search(ret)

    
def start_up_server_if_needed():
    # test if the server is up
    #if test_only: return  # // dzhang_test 
    if not server_is_up():
        write_log ("Cannot connect to MySQL server. Trying to start mysqld!", pstdout=True)
        global master_err_file_handle, master_err_file
        #master_err_file = "%s/var/log/master.err" % mysql_test_dir
        master_err_file = "%s/master.err" % output_dir
        #c2_config_file = "%s/c2_config.cnf" % audit_dir   # enable this in real audit
        c2_config_file = "%s/c2_config.cnf" % mysql_test_dir
        # work around bug 1687, remove log and control files
        #c2_log_dir="/log/databases/tpch"   # this will be defined in c2_config_file
        #sys_call("rm %s/*" % c2_log_dir)
        sys_call ("%s/../sql/mysqld  --core-file --basedir=%s/.. --datadir=%s/var/master-data --language=%s/../sql/share/english --c2db_event_log_level=ERROR --socket=%s --c2db-config-file=%s>& %s &"
              % (mysql_test_dir,mysql_test_dir,mysql_test_dir, mysql_test_dir, mysock, c2_config_file, master_err_file ))
        max_wait_sec = 10
        for i in range(max_wait_sec+1):
            if server_is_up():
                write_log("Mysqld started successfully on sock %s" % mysock,pstdout = True)
                break
            time.sleep(2)
        if (i == max_wait_sec): raise AssertionError, "Time out waiting for mysql server to start\n"
        master_err_file_handle = open(master_err_file)
        if not master_err_file_handle: raise AssertionError, "master.err is not created\n"
    else:
        write_log("MySQL server is up. Use the existing one and continue", pstdout=True)

def shutdown_server():
    if not server_is_up(): return
    dbg_print("Begin shutdown sever")
    sys_call("mysqladmin -u root -S %s shutdown" % mysock)
    max_wait_sec = 20
    for i in range(max_wait_sec+1):
        if not server_is_up():
            write_log("Mysqld is shutdown successfully on sock %s" % mysock, pstdout=True)
            break
        time.sleep(2)
        if (i == max_wait_sec): raise AssertionError, "Time out waiting for mysql server to shutdown\n"

def main(argv):
    ''' may also need to specify seed, mysock. Also may need to take care of loading
    '''
    # default 
    tpch_tables=('customer', 'supplier', 'nation', 'region', 'part', 'partsupp', 'orders', 'lineitem')
    global _debug, scale_factor, do_power, do_throughput, do_update, tpch_db_name, test_only, do_loader, run_id, rand_seed, num_seqs, gen_new_rfs, update_only, no_cleanup, leave_server_up

    os.environ['TZ'] = 'US/Pacific'
    time.tzset()

    try:                                
       opts, args = getopt.gnu_getopt(argv, "hdq:s:r:ptule:", ["help", "debug","query_program=", "scale=", "root=", "power", "throughput", "update","db=","test_only","loader","run_id=","rand_seed=","iter=","gen_rfs","update_only","no_cleanup","leave_server_up"])
    except getopt.GetoptError:          
        usage()                         
        sys.exit(2)                     
    for opt, arg in opts:               
        if opt in ("-h", "--help"):     
            usage()                     
            sys.exit(0)                  
        elif opt == '-d':               
            _debug = 1                  
        elif opt in ("-q", "--query_program"):
            query_exec = arg
        elif opt in ("-s", "--scale"):
            scale_factor = int(arg)
        elif opt in ("-r", "--root"):
            audit_dir= arg
        elif opt in ("-p", "--power"):
            do_power = True
        elif opt in ("-t", "--throughput"):
            do_throughput = True
        elif opt in ("-u", "--update"):
            do_update = True
        elif opt in ("-l", "--loader"):
            do_loader = True
            #print "do update"
        elif opt in ("-e", "--rand_seed"):
            rand_seed = int(arg)
        elif opt in ("--test_only"):
            test_only = True
        elif opt in ("--db"):
            tpch_db_name = arg
        elif opt in ("--run_id"):
            run_id = arg
        elif opt in ("--iter"):
            num_seqs = int(arg)
        elif opt in ("--gen_rfs"):
            gen_new_rfs = True
        elif opt in ("--update_only"):
            update_only = True
            do_update = True
        elif opt in ("--no_cleanup"):
            no_cleanup = True
        elif opt in ("--leave_server_up"):
            leave_server_up = True
    setup_tpch()
    run_tpch_all()

if __name__ == "__main__":
    main(sys.argv[1:])


