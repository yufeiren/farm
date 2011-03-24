/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include "mysplice.h"

int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;

    int filefd;
    if( (filefd = open("in", O_RDONLY) ) < 0 ) {
        perror("open");
        exit(EXIT_FAILURE);
    }

/*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client3");
        exit(1);
    }

/* splice from disk to socket */
	off_t offset;
	offset = 0;
	
	struct stat st;
	fstat(filefd, &st);
	
	ssize_t bytes;
	bytes = 0;
	
	bytes = fs_splice(sockfd, filefd, offset, count);

	printf("bytes send via splice: %ld\n", bytes);

    close(sockfd);
    close(filefd);
    exit(0);
}
