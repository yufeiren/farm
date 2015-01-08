#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
	int i, pgsz;
	char *addr;

	pgsz = getpagesize();
	printf("page size is %d\n", pgsz);

	for (i = 0; i < 3; i++) {
		addr = mmap(NULL, pgsz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (addr == NULL) {
			printf("mmap failed\n");
			exit(-1);
		}
		memset(addr, '\0', pgsz);
		printf("addr[%d] is 0x%lx\n", i, addr);
	}
	
}
