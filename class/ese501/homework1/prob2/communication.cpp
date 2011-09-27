#include "communication.h"
 

void communication::comm() {
	indata_temp = indata.read(); 
	if (clear == 1 || reset ==0) {
		payload_temp = "0000";
		count = 0x00000000;
		error = 0x00000000;
	} else {
		if (indata_temp.range(11,8) == "0001" ) {
		   payload_temp = indata_temp.range(7,4);
		   count = count.read() + 1;
		}
		if (indata_temp[0] == 1) {
		   error = error.read() + 1;
		}
	}

	payload.write(payload_temp);
 
}