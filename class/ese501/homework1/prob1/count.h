#include "systemc.h"

SC_MODULE(count) {
	sc_in<bool> clock;

	sc_in<sc_uint<8> > in1, in2, in3;
	sc_in<bool> load1, load2;
	sc_in<bool> dec1, dec2;
	sc_out<bool> ended;
	sc_out<sc_uint<8> > count1, count2;
	
	void prc_count();
	sc_uint<8> local_count1;
	sc_uint<8> local_count2;
	bool reloaded;
	bool carry_out;		/* indicate overflow */
	
	SC_CTOR(count) {
		SC_METHOD(prc_count);
		sensitive << clock;
	}
};
