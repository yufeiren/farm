/**
 * This application is used to test the current bandwidth
 * of the ftp/ftpd data transfer.
 * 
 * The ftpd process will send a message, contains data length information, to
 * this 'colbw', when it receive some data from ftp client.
 * The message passed by Message Queue IPC. The data structure defined as
 * 'Colbwmesg' type.
 * 
 * Yufei @ 2010/11/14
 *  
 */ 

#include "colbw.h"

void *
anabw(void *arg)
{
	long currlen, datalen;
	
	for ( ; ; ) {
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
	printf("Interval\tTransfer\tBandwidth\n");
	
	double trans, bw;
	char buf[128];
	
	struct tm	*tm_time;
	time_t		t_time;
	
	trans = (double) datalen;
	bw = (double) (datalen) / interval;
	
	/* Interval */
	printf("%d sec\t", interval);
	
	/* Transfer */
	if (trans < KSIZE)	
		printf("\t%.2f Bytes", trans);
	else if (trans < MSIZE)
		printf("\t%.2f KBytes", trans / KSIZE);
	else if (trans < GSIZE)
		printf("\t%.2f MBytes", trans / MSIZE);
	else
		printf("\t%.2f GBytes", trans / GSIZE);
	
	/* Bandwidth */
	if (bw < KSIZE)
		printf("\t%.2f Bytes/sec\n", bw);
	else if (bw < MSIZE)
		printf("\t%.2f KBytes/sec\n", bw / KSIZE);
	else if (bw < GSIZE)
		printf("\t%.2f MBytes/sec\n", bw / MSIZE);
	else
		printf("\t%.2f GBytes/sec\n", bw / GSIZE);
	
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
