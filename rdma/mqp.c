/* mqp: test multiple qp application
 * mqp usage:
 *  -c [server addr]: client side
 *  -s: server side
 *  -d [device name]
 *  -p [port num]
 *  -q [qp number] 
 */

/* procedure:
 * server 
 *   1. listening on TCP port, waiting for connection
 *   2. accept connection
 *   3. create local-qp, set to INIT status 
 *   4. wait for remote-qp information, parse them, set local-qp to RTR, RTS
 *   5. send back local-qp information
 *   6. post task
 * 
 * client
 *   1. connect server
 *   2. create local-qp, set to INIT status
 *   3. send local-qp information
 *   4. wait for remote-qp information, parse them, set local-qp to RTR, RTS
 *   5. post task 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <malloc.h>
#include <getopt.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <infiniband/verbs.h>

struct rdma_context {
	struct ibv_context *context;
	struct ibv_pd      *pd;
	struct ibv_mr      *mr;
	struct ibv_cq      *cq;
	struct ibv_qp      **qp;
	void               *buf;
	unsigned           size;
	int                tx_depth;
	int                *scnt;
	int                *ccnt;
	int                msglen;
	char               msg[1024];
};

static const char *portStates[] = {"Nop","Down","Init","Armed","","Active Defer"};

static int isserver;
static char servaddr[16];
static int port;
static char devname[32];
static int qpnum;

static void runserver();
static void runclient();

static void createqp();

ssize_t             /* Read "n" bytes from a descriptor  */
readn(int fd, void *ptr, size_t n)
{
	size_t		nleft;
	ssize_t		nread;

	nleft = n;
	while (nleft > 0) {
		if ((nread = read(fd, ptr, nleft)) < 0) {
			if (nleft == n)
				return(-1); /* error, return -1 */
			else
				break;      /* error, return amount read so far */
		} else if (nread == 0) {
			break;          /* EOF */
		}
		nleft -= nread;
		ptr   += nread;
	}
	return(n - nleft);      /* return >= 0 */
}

ssize_t             /* Write "n" bytes to a descriptor  */
writen(int fd, const void *ptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;

	nleft = n;
	while (nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) < 0) {
			if (nleft == n)
				return(-1); /* error, return -1 */
			else
				break;      /* error, return amount written so far */
		} else if (nwritten == 0) {
			break;
		}
		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n - nleft);      /* return >= 0 */
}


int
acptconn(short lport)
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	/*  Name the socket.  */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(lport);
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	
	/*  Create a connection queue and wait for clients.  */
	listen(server_sockfd, 5);
	printf("server waiting\n");
	
	/*  Accept a connection.  */
	client_len = sizeof(client_address);
	client_sockfd = accept(server_sockfd, 
	(struct sockaddr *)&client_address, &client_len);
	
	/*  We can now read/write to client on client_sockfd.  */
	return client_sockfd;
}

int
connserv(char *addr, short rport)
{
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	
	/*  Create a socket for the client.  */
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	/*  Name the socket, as agreed with the server.  */
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(addr);
	address.sin_port = htons(rport);
	len = sizeof(address);
	
	/*  Now connect our socket to the server's socket.  */
	
	result = connect(sockfd, (struct sockaddr *)&address, len);
	
	if(result == -1) {
		perror("connect fail\n");
		exit(1);
	}
	
	return sockfd;
}

int
main(int argc, char **argv)
{
	/* get parameter */
	int c;
	port = 8301;
	isserver = 0;
	qpnum = 1;

	while ((c = getopt(argc, argv, "c:sd:p:q:")) != -1) {
		switch (c) {
		case 'c':
			memset(servaddr, '\0', 16);
			strcpy(servaddr, optarg);
			printf("is client: connect to [%s]\n", servaddr);
			break;
		case 's':
			isserver = 1;
			printf("is server\n");
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 'd':
			memset(devname, '\0', 32);
			strcpy(devname, optarg);
			break;
		case 'q':
			qpnum = atoi(optarg);
			break;
		default:
			printf("wrong parameter\n");
			break;
		}
	}
	
	srand48(getpid() * time(NULL));

	/* launch client/server */
	if (isserver)
		runserver();
	else
		runclient();

	exit(EXIT_SUCCESS);
}


static struct rdma_context *create_ctx(char *ib_devname)
{
	struct rdma_context *ctx;
	struct ibv_device *ib_dev = NULL;
	
	struct ibv_context *context;
	
	int num_of_device;
	struct ibv_device **dev_list;
	int ib_port = 1;
	
	struct ibv_port_attr port_attr;
	
	char buf[1024];
	
	ctx = (struct rdma_context *) malloc(sizeof(struct rdma_context));
	if (ctx == NULL) {
		printf("malloc fail\n");
		exit(EXIT_FAILURE);
	}
	
	memset(ctx, '\0', sizeof(struct rdma_context));
	ctx->msglen = 0;
	
	/* get device */
	dev_list = ibv_get_device_list(&num_of_device);

	if (num_of_device <= 0) {
		fprintf(stderr," Did not detect devices \n");
		fprintf(stderr," If device exists, check if driver is up\n");
		exit(EXIT_FAILURE);
	}

	if (!ib_devname) {
		ib_dev = dev_list[0];
		if (ib_dev == NULL)
			fprintf(stderr, "No IB devices found\n");
	} else {
		for (; (ib_dev = *dev_list); ++dev_list)
			if (!strcmp(ibv_get_device_name(ib_dev), ib_devname))
				break;
		if (!ib_dev)
			fprintf(stderr, "IB device %s not found\n", ib_devname);
	}
	
	/* get context */
	ctx->context = ibv_open_device(ib_dev);
	if (!ctx->context) {
		fprintf(stderr, "Couldn't get context for %s\n",
			ibv_get_device_name(ib_dev));
		exit(EXIT_FAILURE);
	}
	
	/* get gid */
	union ibv_gid temp_gid;
	if (ibv_query_gid(ctx->context, 1, 0, &temp_gid)) {
		fprintf(stderr, "ibv_query_gid fail");
		exit(EXIT_FAILURE);
	}
	
	memset(buf, '\0', 1024);
	sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", \
			temp_gid.raw[0],temp_gid.raw[1],
			temp_gid.raw[2],temp_gid.raw[3],
			temp_gid.raw[4],temp_gid.raw[5],
			temp_gid.raw[6],temp_gid.raw[7],
			temp_gid.raw[8],temp_gid.raw[9],
			temp_gid.raw[10],temp_gid.raw[11],
			temp_gid.raw[12],temp_gid.raw[13],
			temp_gid.raw[14],temp_gid.raw[15]);
	memcpy(ctx->msg, buf, 47);
	ctx->msglen += 47;
	
/*	if (ibv_query_port(context, port, &port_attr) != 0) { */
	if (ibv_query_port(ctx->context, 1, &port_attr) != 0) {
		fprintf(stderr, "ibv_query_port: %m");
		exit(EXIT_FAILURE);
	}
	
	if (port_attr.state != IBV_PORT_ACTIVE) {
		fprintf(stderr, " Port number %d state is %s\n", \
			ib_port, portStates[port_attr.state]);
		exit(EXIT_FAILURE);
	}
	
	/* create pd */
	ctx->pd = ibv_alloc_pd(ctx->context);
	if (!ctx->pd) {
		fprintf(stderr, "Couldn't allocate PD\n");
		exit(EXIT_FAILURE);
	}
	
	/* create memory region */
	ctx->mr = ibv_reg_mr(ctx->pd, ctx->buf, 1024, \
			IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_LOCAL_WRITE);
	if (!ctx->mr) {
		fprintf(stderr, "Couldn't allocate MR\n");
		exit(EXIT_FAILURE);
	}
	
	/* create cq */
	ctx->cq = ibv_create_cq(ctx->context, 100, NULL, NULL, 0);
	if (!ctx->cq) {
		fprintf(stderr, "Couldn't create CQ\n");
		exit(EXIT_FAILURE);
	}
	
	/* create qp */
	int i;
	struct ibv_qp_init_attr attr;
	struct ibv_qp_attr qp_attr;
	int flags = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT;

	ctx->qp = (struct ibv_qp *) malloc(sizeof(struct ibv_qp *) * qpnum);
	if (!ctx->qp) {
		fprintf(stderr, "Couldn't malloc qp\n");
		exit(EXIT_FAILURE);
	}
	
	/* modify qp */
	for (i = 0; i < qpnum; i++) {
		memset(&attr, 0, sizeof(struct ibv_qp_init_attr));
		attr.send_cq = ctx->cq;
		attr.recv_cq = ctx->cq; 
		attr.cap.max_send_wr  = 100;
		attr.cap.max_recv_wr  = 1;
		attr.cap.max_send_sge = 1;
		attr.cap.max_recv_sge = 1;
		attr.cap.max_inline_data = 0;
		
		attr.qp_type = IBV_QPT_RC;
	
		ctx->qp[i] = ibv_create_qp(ctx->pd, &attr);
		if (!ctx->qp[i])  {
			fprintf(stderr, "Couldn't create QP\n");
			exit(EXIT_FAILURE);
		}
		
		memset(buf, '\0', 1024);
		sprintf(buf, "%04x:%04x:%06x:%06x", \
			port_attr.lid, 4, \
			ctx->qp[i]->qp_num, lrand48() & 0xffffff);
		
		memcpy(ctx->msg + 47 + i * 23, buf, 23);
		ctx->msglen += 47;
		
		memset(&qp_attr, 0, sizeof(struct ibv_qp_attr));
		qp_attr.qp_state        = IBV_QPS_INIT;
		qp_attr.pkey_index      = 0;
		qp_attr.port_num        = ib_port;
		qp_attr.qp_access_flags = IBV_ACCESS_REMOTE_WRITE;

		flags |= IBV_QP_ACCESS_FLAGS;

		if (ibv_modify_qp(ctx->qp[i], &qp_attr, flags))  {
			fprintf(stderr, "Failed to modify QP to INIT\n");
			exit(EXIT_FAILURE);
		}
	}
	
	return ctx;
}


static void
runserver()
{
	struct rdma_context *ctx;
	/* accept a new connection */
	int connfd;
	if ((connfd = acptconn((short) port)) < 0) {
		printf("accept fail\n");
		exit(EXIT_FAILURE);
	}
	
	/* create local-qp, set to INIT status */
	ctx = create_ctx(devname);
	if (!ctx) {
		printf("create_ctx fail\n");
		exit(EXIT_FAILURE);	
	}
	
	/* write ctx->msg */
	writen(connfd, ctx->msg, ctx->msglen);
	
	/* read remote side msg */
	readn(connfd, ctx->msg, ctx->msglen);
	
	return;
}


static void
runclient()
{
	struct rdma_context *ctx;
	
	int connfd;
	if ((connfd = connserv(servaddr, (short) port)) < 0) {
		printf("connect fail\n");
		exit(EXIT_FAILURE);
	}
	
	/* create local-qp, set to INIT status */
	ctx = create_ctx(devname);
	if (!ctx) {
		printf("create_ctx fail\n");
		exit(EXIT_FAILURE);	
	}
	
	/* write ctx->msg */
	writen(connfd, ctx->msg, ctx->msglen);
	
	/* read remote side msg */
	readn(connfd, ctx->msg, ctx->msglen);
	
	return;
}
