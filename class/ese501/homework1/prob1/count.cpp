#include "count.h"

void count::prc_count() {	
	carry_out = false;

	if (load1.read() == 1) {
		local_count1 = in1.read();
		
		if (dec1.read() == 1) {
			if (local_count1 == 0)
				carry_out = true;
			local_count1 = in1.read() - 1;
		}
	}
	
	if (load2.read() == 1) {
		local_count2 = in2.read();
		
		if (dec2.read() == 1) {
			if (local_count2 < in3.read())
				carry_out = true;
			local_count2 = in2.read() - in3.read();
		}
	}
	
	if ((local_count1 == local_count2) || (carry_out == true)) {
		ended.write(1);
	} else {
		ended.write(0);
	}
	
	count1.write(local_count1);
	count2.write(local_count2);
}
