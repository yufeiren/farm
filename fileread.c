#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU
#include <fcntl.h>
#include <string.h>

int
main(int argc, char **argv)
{

	char *buf;
	int ret = 0;
	long tl = 0;
	int cl = 0;
	int fd;
	
/*	int ps = getpagesize();
	printf("pagesize is: %d\n", ps);
	
	unsigned long long int bytes_written = 0;
	
	
	if( (ret = posix_memalign(&buf, ps, ps*256)) != 0 ) {
		perror("Memalign failed");
		exit(ret);
	} */
	
	buf = (char *) malloc(4096);
	if (buf == NULL) {
		perror("malloc fail");
		exit(1);
	}
	
	memset(buf, 0x00, 4096);
	
	if( (fd = open(argv[1], O_RDONLY) ) < 0 ) {
		perror("Open failed");
		exit(ret);
	}
	
	while ( (cl = read(fd, buf, 4096)) > 0)
		tl += cl;
	
	printf("%s: finish read, total len [%ld].\n", argv[0], tl);
	close(fd);
	free(buf);
}