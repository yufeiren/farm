#include "driver.h"

void driver::prc_driver() {
	sc_uint<3> input;
	input = 0;
	while (1) {
		d_a.write(1);
		d_b.write(1);
		wait(5, SC_NS);
	}
}
