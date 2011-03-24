#include "mysplice.h"

ssize_t
fs_splice(int out_fd, int in_fd, off_t offset, size_t count)
{
	int pipefd[2];
	
	ssize_t bytes, bytes_sent, bytes_in_pipe;
	size_t total_bytes_sent = 0;

	if ( pipe(pipefd) < 0 ) {
		perror("pipe");
		return -1;
	}
	
	// Splice the data from in_fd into the pipe
	while (total_bytes_sent < count) {
		if ((bytes_sent = splice(in_fd, NULL, pipefd[1], NULL,
			count - total_bytes_sent,
			SPLICE_F_MORE | SPLICE_F_MOVE)) <= 0) {
			if (errno == EINTR || errno == EAGAIN) {
				// Interrupted system call/try again
				// Just skip to the top of the loop and try again
				continue;
			}
			perror("splice");
			close(pipefd[0]);
			close(pipefd[1]);
			return -1;
		}
	
		// Splice the data from the pipe into out_fd
		bytes_in_pipe = bytes_sent;
		while (bytes_in_pipe > 0) {
			if ((bytes = splice(pipefd[0], NULL, out_fd, NULL, bytes_in_pipe,
				SPLICE_F_MORE | SPLICE_F_MOVE)) <= 0) {
				if (errno == EINTR || errno == EAGAIN) {
					// Interrupted system call/try again
					// Just skip to the top of the loop and try again
					continue;
				}
				perror("splice");
				close(pipefd[0]);
				close(pipefd[1]);
				return -1;
			}
			bytes_in_pipe -= bytes;
		}
	
		total_bytes_sent += bytes_sent;
	}
	
	close(pipefd[0]);
	close(pipefd[1]);
	return total_bytes_sent;
}


ssize_t
sf_splice(int out_fd, int in_fd, off_t offset, size_t count)
{
	int pipefd[2];
	
	ssize_t bytes, bytes_recv, bytes_in_pipe;
	size_t total_bytes_recv = 0;

	if ( pipe(pipefd) < 0 ) {
		perror("pipe");
		return -1;
	}
	
	size_t splice_block_size = 16384;	/* 16KB */
	
	// Splice the data from in_fd into the pipe
	while ((count == 0) || (total_bytes_recv < count)) {
		if ((bytes_recv = splice(in_fd, NULL, pipefd[1], NULL,
			splice_block_size,
			SPLICE_F_MORE | SPLICE_F_MOVE)) < 0) {
			if (errno == EINTR || errno == EAGAIN) {
				// Interrupted system call/try again
				// Just skip to the top of the loop and try again
				continue;
			}
			perror("splice");
			close(pipefd[0]);
			close(pipefd[1]);
			return -1;
		} else if (bytes_recv == 0)
			break;
		
		// Splice the data from the pipe into out_fd
		bytes_in_pipe = bytes_recv;
		while (bytes_in_pipe > 0) {
			if ((bytes = splice(pipefd[0], NULL, out_fd, &offset, bytes_in_pipe,
				SPLICE_F_MORE | SPLICE_F_MOVE)) <= 0) {
				if (errno == EINTR || errno == EAGAIN) {
					// Interrupted system call/try again
					// Just skip to the top of the loop and try again
					continue;
				}
				perror("splice");
				close(pipefd[0]);
				close(pipefd[1]);
				return -1;
			}
			bytes_in_pipe -= bytes;
		}
	
		total_bytes_recv += bytes_recv;
	}
	
	close(pipefd[0]);
	close(pipefd[1]);
	return total_bytes_recv;
}