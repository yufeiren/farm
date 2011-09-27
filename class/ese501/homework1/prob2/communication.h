#include "systemc.h"
 
SC_MODULE (communication) {
	sc_in<bool> clock;
	sc_in<bool> reset;
	sc_in<bool> clear;
	sc_in<sc_bv<12> > indata;
	
	sc_out<sc_bv<4> >   payload;
	sc_out<sc_uint<8> >   count;
	sc_out<sc_uint<8> >   error;
	
	sc_bv<4>  payload_temp;
	sc_bv<12>  indata_temp;
	
	
	void comm();
	
	SC_CTOR(communication) {
		SC_METHOD(comm);
		sensitive <<clock  ;
	}
};