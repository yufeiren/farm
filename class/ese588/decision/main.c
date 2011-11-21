#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dt.h"

/* ./dt [raw-file] */

static void
usage(const char *name)
{
	printf("usage: %s [-i input_file] [-s min_sup] [-c min_confidence]\n", \
		name);
	printf("\t-i\t\tInput File\n");
	printf("\t-s\t\tMinimum Support - default 10\n");
	printf("\t-c\t\tMinimum Confidence - default 20\n");
}

int
main(int argc, char **argv)
{
	int total_trans;
	int op;
	char inputfile[1024];
	
	memset(inputfile, '\0', 1024);
	while ( (op = getopt(argc, argv, "i:")) != -1) {
		switch (op) {
		case 'i':
			strcpy(inputfile, optarg);
			break;
		default:
			usage(argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}
	
	if (strlen(inputfile) == 0) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	TAILQ_INIT(&adult_rec_tqh);
	
	total_trans = loaddata_adult_txt(inputfile);
	DPRINTF(("total trans is %d\n", total_trans));
	
	return 0;
}
