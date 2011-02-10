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
#include <assert.h>
#include <stdint.h>
#include <linux/limits.h>

typedef uint64_t max_size_t;

max_size_t totallen;		/* totally received data length */

long timestamp;

int interval;
int last;

void *anabw(void *);

void prtinfo(long);

void calspeed(time_t, max_size_t);

const long kKilo_to_Unit = 1024;
const long kMega_to_Unit = 1024 * 1024;
const long kGiga_to_Unit = 1024 * 1024 * 1024;

const long kkilo_to_Unit = 1000;
const long kmega_to_Unit = 1000 * 1000;
const long kgiga_to_Unit = 1000 * 1000 * 1000;

static void
usage(const char *msg)
{
	printf(
"usage: fiotest [ options ]\n"
"options: -r \t\t\treadfile, read() default\n"
"         -w \t\t\twritefile, write() default\n"
"         -o \t\t\tO_DIRECT or not\n"
"         -s \t\t\tstream fread/fwrite or not\n"
"         -f <file name>\n"
"         -m <r/w memory buffer/block size>\n"
"         -t <filesize totally, for write operation>\n"
"         -i <statistical interval time, default 2 sec>\n"
);

	if (msg[0] != 0)
		fprintf(stderr, "%s\n", msg);
	exit(EXIT_FAILURE);
}

max_size_t byte_atoi(const char *);

int
main(int argc, char **argv)
{

	int ret = 0;
	int cl = 0;
	
	pthread_t tid;
	interval = 2;
	
	time_t startt, finisht;
	
	int isread = 0;
	int iswrite = 0;
	int isodirect = 0;
	int isstream = 0;
	char filename[128];
	FILE *fp;
	int fd;
	char *buf;
/* 	void *obuf;	used for odirect */
	max_size_t bufsiz = 0;
	max_size_t filesize = 0;
	
	last = 0;
	
	strcpy(filename, "./out"); /* default write file name */
	
	/* get parameter */
	int c;
	
	while ((c = getopt(argc, argv, "rwosf:m:t:i:")) != -1) {
		switch (c) {
		case 'r':
			isread = 1;
			printf("init: read file\n");
			break;
		case 'w':
			iswrite = 1;
			printf("init: write file\n");
			break;
		case 'o':
			printf("init: odirect is open\n");
			isodirect = 1;
			break;
		case 's':
			printf("init: use stream io\n");
			isstream = 1;
			break;
		case 'f':
			memset(filename, '\0', 128);
			strcpy(filename, optarg);
			printf("init: file name is %s\n", filename);
			break;
		case 'm':
			bufsiz = byte_atoi(optarg);
			printf("init: buffer size is %ld\n", bufsiz);
			break;
		case 't':
			filesize = byte_atoi(optarg);
			printf("init: file size is %ld\n", filesize);
			break;
		case 'i':
			interval = atoi(optarg);
			break;
		default:
			usage("wrong parameter\n");
			break;
		}
	}
	
	
	/* alloc buffer */
		/* odirect */
	if (isodirect == 1) {
		int ps = getpagesize();
		printf("O_DIRECT: pagesize is: %d\n", ps);

		/* memory size should be devided to pagesize ?? */
		if (bufsiz < ps)
			bufsiz = ps;
		else
			bufsiz = (bufsiz / ps) * ps;
		printf("O_DIRECT: new pagesize is: %d\n", bufsiz);
		if( (ret = posix_memalign(&buf, ps, bufsiz)) != 0 ) {
			perror("Memalign failed");
			exit(ret);
		}
	} else {
		buf = (char *) malloc(bufsiz);
		if (buf == NULL) {
			perror("malloc fail");
			exit(1);
		}
	}
	
	memset(buf, 0xaa, bufsiz);
	
	/* create calspd thread */
	if (interval != 0) {
		ret = pthread_create(&tid, NULL, anabw, NULL);
		if (ret != 0) {
			printf("Can't create thread: %s\n", strerror(ret));
			exit(EXIT_FAILURE);
		}
	}
	
	/* open file */
	if (isread == 1) {
		if (isodirect == 1) {
			if( (fd = open(filename, O_RDONLY | O_DIRECT) ) < 0 ) {
				perror("Open failed");
				exit(ret);
			}
		} else if (isstream == 1) {
			if( (fp = fopen(filename, "r") ) < 0 ) {
				perror("Open failed");
				exit(ret);
			}
		} else {
			if( (fd = open(filename, O_RDONLY) ) < 0 ) {
				perror("Open failed");
				exit(ret);
			}
		}
	} else {
		if (isodirect == 1) {
			if( (fd = open(filename, O_CREAT |O_WRONLY | O_DIRECT) ) < 0 ) {
				perror("Open failed");
				exit(ret);
			}
		} else if (isstream == 1) {
			if( (fp = fopen(filename, "w") ) < 0 ) {
				perror("Open failed");
				exit(ret);
			}
		} else {
			if( (fd = open(filename, O_CREAT | O_WRONLY) ) < 0 ) {
				perror("Open failed");
				exit(ret);
			}
		}
	}
	
	
	time(&startt);
	/* file operation */
	if (isread == 1) {
		if (isstream == 1) {
			while ( (cl = fread(buf, bufsiz, 1, fp)) > 0)
				totallen += cl * bufsiz;
		} else {
			while ( (cl = read(fd, buf, bufsiz)) > 0)
				totallen += cl;
		}
		
		printf("finish read, total len [%ld].\n", totallen);
	} else {
		max_size_t tail = 0;
		while ( totallen < filesize ) {
			tail = filesize - totallen;
			if (tail < bufsiz)
				bufsiz = tail;
			if (isstream == 1) {
				if ( (cl = fwrite(buf, bufsiz, 1, fp)) > 0)
					totallen += cl * bufsiz;
			} else {
				if ( (cl = write(fd, buf, bufsiz)) > 0)
					totallen += cl;
			}
		}
		printf("finish write, total len [%ld].\n", totallen);
	}
	time(&finisht);
	
	calspeed(finisht - startt, totallen);
	if (isstream == 1)
		fclose(fp);
	else
		close(fd);
	
	free(buf);
	
	exit(0);
}

max_size_t byte_atoi( const char *inString ) {
    double theNum;
    char suffix = '\0';

    assert( inString != NULL );

    /* scan the number and any suffices */
    sscanf( inString, "%lf%c", &theNum, &suffix );

    /* convert according to [Gg Mm Kk] */
    switch ( suffix ) {
        case 'G':  theNum *= kGiga_to_Unit;  break;
        case 'M':  theNum *= kMega_to_Unit;  break;
        case 'K':  theNum *= kKilo_to_Unit;  break;
        case 'g':  theNum *= kgiga_to_Unit;  break;
        case 'm':  theNum *= kmega_to_Unit;  break;
        case 'k':  theNum *= kkilo_to_Unit;  break;
        default: break;
    }
    return (max_size_t) theNum;
} /* end byte_atoi */


void *
anabw(void *arg)
{
	long currlen, datalen;
	
	printf("Interval\tTransfer\tBandwidth\n");
	for ( ; ; ) {
		
		if ( ++last % 10 == 0)
			printf("Interval\tTransfer\tBandwidth\n");
		
		currlen = totallen;
		
		sleep(interval);
		
		datalen = totallen - currlen;
		
		prtinfo(datalen);
	}
	
	pthread_exit(NULL);
}

void
prtinfo(long datalen)
{
	
	double trans, bw;
	char buf[128];
	
	struct tm	*tm_time;
	time_t		t_time;
	
	trans = (double) datalen;
	bw = (double) (datalen) / interval;
	
	/* Interval */
	printf("%d sec\t", interval);
	
	/* Transfer */
	if (trans < kKilo_to_Unit)	
		printf("\t%.2f Bytes", trans);
	else if (trans < kMega_to_Unit)
		printf("\t%.2f KBytes", trans / kKilo_to_Unit);
	else if (trans < kGiga_to_Unit)
		printf("\t%.2f MBytes", trans / kMega_to_Unit);
	else
		printf("\t%.2f GBytes", trans / kGiga_to_Unit);
	
	/* Bandwidth */
	if (bw < kKilo_to_Unit)
		printf("\t%.2f Bytes/sec\n", bw);
	else if (bw < kMega_to_Unit)
		printf("\t%.2f KBytes/sec\n", bw / kKilo_to_Unit);
	else if (bw < kGiga_to_Unit)
		printf("\t%.2f MBytes/sec\n", bw / kMega_to_Unit);
	else
		printf("\t%.2f GBytes/sec\n", bw / kGiga_to_Unit);
	
	/* output to logfile - time bw
	memset(buf, '\0', 128); */
	
/* 	time(&t_time);
	tm_time = localtime(&t_time);
	HH:MM:SS - 8 byte
	snprintf(buf, 7, "%02d%02d%02d", \
		tm_time->tm_hour, \
		tm_time->tm_min, \
		tm_time->tm_sec); */
	
/*	timestamp += interval;
	sprintf(buf, "%ld %.2f\n", timestamp, (double) bw / GSIZE);
	
	fprintf(bwfp, "%s %.2f\n", \
		asctime(localtime(&tloc)), (double) bw / GSIZE);
	
	write(fd, buf, strlen(buf)); */
	
	return;
}

void
calspeed(time_t inter, max_size_t len)
{
	double speed = (double) len / inter;
	
	printf("[SUM]: %ld Bytes in %d sec: ", len, inter);
	if (speed < kKilo_to_Unit)
		printf("\t%.2f Bytes/sec\n", speed);
	else if (speed < kMega_to_Unit)
		printf("\t%.2f KBytes/sec\n", speed / kKilo_to_Unit);
	else if (speed < kGiga_to_Unit)
		printf("\t%.2f MBytes/sec\n", speed / kMega_to_Unit);
	else
		printf("\t%.2f GBytes/sec\n", speed / kGiga_to_Unit);
	
	return;
}
