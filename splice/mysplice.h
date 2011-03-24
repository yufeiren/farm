
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#ifndef __USE_GNU
#define __USE_GNU		/* for splice */
#endif
#include <fcntl.h>

ssize_t fs_splice(int out_fd, int in_fd, off_t offset, size_t count);

ssize_t sf_splice(int out_fd, int in_fd, off_t offset, size_t count);
