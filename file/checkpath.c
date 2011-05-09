#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void checkpath(char *path)
{
	struct stat statbuf;
	
	lstat(path, &statbuf);
	
	if (S_ISDIR(statbuf.st_mode)) {
		printf("IS DIR\n");
	} else if (S_ISREG(statbuf.st_mode)) {
		printf("IS REG\n");
	} else if (S_ISCHR(statbuf.st_mode)) {
		printf("IS CHR\n");
	} else if (S_ISBLK(statbuf.st_mode)) {
		printf("IS BLK\n");
	} else if (S_ISFIFO(statbuf.st_mode)) {
		printf("IS FIFO\n");
	} else if (S_ISLNK(statbuf.st_mode)) {
		printf("IS LNK\n");
	} else if (S_ISSOCK(statbuf.st_mode)) {
		printf("IS SOCK\n");
	} else {
		printf("unknown file type\n");
	}
	
	return;
}

/*  Now we move onto the main function.  */

int main(int argc, char* argv[])
{
    char *topdir, pwd[2]=".";
    if (argc != 2)
        topdir=pwd;
    else
        topdir=argv[1];

    printf("the path of %s\n",topdir);
    checkpath(topdir);
    printf("done.\n");

    exit(0);
}

