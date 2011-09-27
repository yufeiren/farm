#include "communication.h"
#include "test.h"

int sc_main (int argc, char* argv[])
{

	sc_signal<bool> reset;
	sc_signal<bool> clear;
	sc_signal<sc_bv<12> > indata;
	sc_signal<sc_bv<4> > payload;
	sc_signal<sc_uint<8> > count;
	sc_signal<sc_uint<8> > error;
	
	sc_clock clock("Clock", 10, SC_NS, false);
	
	// Connect the modules
	test Tst("test");
	Tst.reset(reset);
	Tst.clear(clear);
	Tst.indata(indata);

	communication Comm("communication");
	Comm.clock(clock);
	Comm.reset(reset);
	Comm.clear(clear);
	Comm.indata(indata);
	Comm.payload(payload);
	Comm.count(count);
	Comm.error(error);
    
	sc_trace_file *wf = sc_create_vcd_trace_file("Communication_output");
	
	sc_trace(wf, Comm.clock, "clock");
	sc_trace(wf, Comm.reset, "reset");
	sc_trace(wf, Comm.clear, "clear");
	sc_trace(wf, Comm.indata, "indata");
	sc_trace(wf, Comm.payload, "payload");
	sc_trace(wf, Comm.count, "count");
	sc_trace(wf, Comm.error, "error");
	
	sc_start(700,SC_NS);
	
	sc_close_vcd_trace_file(wf);
	sc_stop();
	
	return 0;
}