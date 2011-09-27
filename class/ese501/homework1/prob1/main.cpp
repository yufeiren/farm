#include "count.h"
#include "test.h"

int sc_main(int argc, char **argv)
{
	sc_signal<sc_uint<8> > t_in1, t_in2, t_in3;
	sc_signal<bool> t_load1, t_load2;
	sc_signal<bool> t_dec1, t_dec2;
	sc_signal<bool> t_ended;
	sc_signal<sc_uint<8> > t_count1, t_count2;
	
	sc_set_time_resolution(1, SC_NS);
	sc_set_default_time_unit(1, SC_NS);
	sc_clock Clk("clock", 10, SC_NS, false);
	
	count Cnt("Cnt");
	Cnt.clock(Clk);
	Cnt.in1(t_in1);
	Cnt.in2(t_in2);
	Cnt.in3(t_in3);
	Cnt.load1(t_load1);
	Cnt.load2(t_load2);
	Cnt.dec1(t_dec1);
	Cnt.dec2(t_dec2);
	Cnt.ended(t_ended);
	Cnt.count1(t_count1);
	Cnt.count2(t_count2);
	
	test Tst("Tst");
	Tst.in1(t_in1);
	Tst.in2(t_in2);
	Tst.in3(t_in3);
	Tst.load1(t_load1);
	Tst.load2(t_load2);
	Tst.dec1(t_dec1);
	Tst.dec2(t_dec2);
	
	sc_trace_file *tf = sc_create_vcd_trace_file("wave");
	
	sc_trace(tf, Cnt.clock, "clock");
	sc_trace(tf, Cnt.in1, "in1");
	sc_trace(tf, Cnt.in2, "in2");
	sc_trace(tf, Cnt.in3, "in3");
	sc_trace(tf, Cnt.load1, "load1");
	sc_trace(tf, Cnt.load2, "load2");
	sc_trace(tf, Cnt.dec1, "dec1");
	sc_trace(tf, Cnt.dec1, "dec2");
	sc_trace(tf, Cnt.ended, "ended");
	sc_trace(tf, Cnt.count1, "count1");
	sc_trace(tf, Cnt.count2, "count2");

	sc_start(3500, SC_NS);
	
	sc_close_vcd_trace_file(tf);
	
	sc_stop();

	return 0;
}
