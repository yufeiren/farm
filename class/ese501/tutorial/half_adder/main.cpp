#include "half_adder.h"
#include "driver.h"

int sc_main(int argc, char **argv)
{
	sc_signal<bool> t_a, t_b, t_sum, t_carry;

	half_adder ha("myadder");
	ha << t_a << t_b << t_sum << t_carry;

	driver d1("mydriver");
	d1.d_a(t_a);
	d1.d_b(t_b);
	
	sc_start(100, SC_NS);

	return 0;
}
