#include "test.h"

void test::prc_test() {
    in1 = 0;
    in2 = 0;
    in3 = 0;
    load1 = false;
    load2 = false;
    dec1 = false;
    dec2 = false;
    wait(40, SC_NS);
  	in1 = 0x50;
  	in2 = 0x5F;
  	in3 = 0x10;
        load1 = true;
    wait(400, SC_NS);
  	dec1 = true;
        load2 = true;
    wait(800, SC_NS);
  	dec2 = true;
    wait(400, SC_NS);
  	in1 = 0x08;
  	in2 = 0x05;
  	in3 = 0x02;
    wait(400, SC_NS);
        dec1 = false;
        in2 = 0x28;
        in3 = 0x21;
    wait(400, SC_NS);
        in3 = 0x20;	
    wait(400, SC_NS);
        in3 = 0x23;	
    wait(400, SC_NS);
        in3 = 0x5F;
}
