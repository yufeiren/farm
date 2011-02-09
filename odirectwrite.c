#define _XOPEN_SOURCE 600
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

	void *buf;
	int ret = 0;
	int cl = 0;
	
	pthread_t tid;
	interval = 2;
	
	long ofilesize;
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s <filename> <filesize>\n", argv[0]);
		exit(ret);
	}
		
	
	ofilesize = atoi(argv[2]);
	
	int ps = getpagesize();
	printf("pagesize is: %d\n", ps);
	
	unsigned long long int bytes_written = 0;
	int fd;
	
	if( (ret = posix_memalign(&buf, ps, ps*256)) != 0 ) {
		perror("Memalign failed");
		exit(ret);
	}
	
	memset(buf, 0x00, ps*256);
	
	ret = pthread_create(&tid, NULL, anabw, NULL);
	if (ret != 0) {
		printf("Can't create thread: %s\n", strerror(ret));
		exit(EXIT_FAILURE);
	}
	
	if( (fd = open(argv[1], O_CREAT | O_WRONLY | O_DIRECT) ) < 0 ) {
		perror("Open failed");
		exit(ret);
	}
	
	while ( totallen < ofilesize )
		if ( (cl = write(fd, buf, ps*256)) > 0)
			totallen += cl;
	
	printf("%s: finish write, total len [%ld].\n", argv[0], totallen);
	close(fd);
	free(buf);

	
}
