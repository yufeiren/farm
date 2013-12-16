/* gcc test_adler32.c -lz -o adler32 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <zlib.h>

int main()
{
	char buf[1024];
	uint32_t adler;

	memset(buf, '\0', 1024);
	adler = adler32(0L, buf, 1024);

	printf("alder is 0x%x", adler);
	return 0;
}

