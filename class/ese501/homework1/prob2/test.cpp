#include "test.h"

void test::prc_test() {
	reset = 1;
	clear = 1;
	wait(40, SC_NS);
	clear = 0;
	indata = 0x1F1;
	wait(80, SC_NS);
	indata = 0x0E0;
	wait(80, SC_NS);
	indata = 0x170;
	wait(80, SC_NS);
	indata = 0x0E0;
	wait(400, SC_NS);
	clear = 1;
}