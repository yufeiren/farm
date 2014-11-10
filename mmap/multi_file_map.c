#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int i;
	char *addr, *start_addr, *next_addr;

	char *prefix;
	int size;
	int fd;
	int num_files;
	char f[1024];

	if (argc < 4)
		printf("Usage: %s prefix #_of_files size", argv[0]);

	prefix = argv[1];
	num_files = atoi(argv[2]);
	size = atoi(argv[3]);

	addr = NULL;
	start_addr = NULL;
	next_addr = NULL;

	for (i = 0; i < num_files; i++) {
		memset(f, '\0', sizeof(f));
		sprintf(f, "%s.%d", prefix, i);

		/* open file */
		fd = open(f, O_RDWR | O_CREAT, (mode_t)0600);
		if (fd == -1) {
			printf("can not open file: %s\n", f);
			exit(-1);
		}

		addr = mmap(next_addr, size, PROT_READ | PROT_WRITE, \
			    MAP_SHARED, fd, 0);
		if (addr == MAP_FAILED) {
			printf("mmap failed\n");
			exit(-1);
		}
		memset(addr, '\0', size);
		start_addr = addr;
		next_addr = addr - size;
		printf("new start_addr is 0x%lx\n", start_addr);

		//		close(fd);
	}

	for (i = 0; i < 3; i++) {
		printf("set addr 0x%lx\n", start_addr + i * size);
		memset(start_addr + i * size, 'a' + i, size);

		if (msync(start_addr + i * size, size, MS_SYNC) == -1) {
			printf("Could not sync the file to disk");
		}
	}

	return 0;
}

