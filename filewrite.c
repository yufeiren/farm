#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU
#include <fcntl.h>
#include <string.h>

#include "calspd.h"

int
main(int argc, char **argv)
{

	char *buf;
	int ret = 0;
	long tl = 0;
	int cl = 0;
	int fd;
	
	pthread_t tid;
	interval = 2;
	
	long ofilesize;
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s <filename> <filesize>\n", argv[0]);
		exit(ret);
	}
		
	
	ofilesize = atol(argv[2]);
	
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
	
	ret = pthread_create(&tid, NULL, anabw, NULL);
	if (ret != 0) {
		printf("Can't create thread: %s\n", strerror(ret));
		exit(EXIT_FAILURE);
	}
	
	if( (fd = open(argv[1], O_RDONLY) ) < 0 ) {
		perror("Open failed");
		exit(ret);
	}
	
	while ( totallen < ofilesize )
		if ( (cl = write(fd, buf, 4096)) > 0)
			totallen += cl;
	
	printf("%s: finish write, total len [%ld].\n", argv[0], totallen);
	close(fd);
	free(buf);
}
