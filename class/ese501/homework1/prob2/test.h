#include "systemc.h"

SC_MODULE(test) {
	sc_out<bool> reset;
	sc_out<bool> clear;
	sc_out<sc_bv<12> > indata;
	
	void prc_test();
	
	SC_CTOR(test) {
		SC_THREAD(prc_test);
	}

};