#include "systemc.h"

SC_MODULE(test) {
	sc_out<sc_uint<8> > in1, in2, in3;
	sc_out<bool> load1, load2;
	sc_out<bool> dec1, dec2;
	
	void prc_test();
	
	SC_CTOR(test) {
		SC_THREAD(prc_test);
	}
};