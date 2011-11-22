#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dt.h"

/* ./dt [-i raw-file] */

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
	int i;
	
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

	/* possible value for each attribute rage */
	attr_range[0] = 9;
	attr_range[1] = 17;
	attr_range[2] = 9;
	attr_range[3] = 15;
	attr_range[4] = 7;
	attr_range[5] = 6;
	attr_range[6] = 3;
	attr_range[7] = 42;
	attr_range[8] = 3;

	/* create root node */
	Dt_node *root = (Dt_node *) malloc(sizeof(Dt_node));
	memset(root, '\0', sizeof(Dt_node));
	TAILQ_INIT(&root->adult_rec_tqh);
	/* all the attribute is valid */
	for (i = 0; i < ADULT_ATTRIBUTE_NUM; i ++) {
		root->node_attr_list.attr_is_valid[i] = 1;
	}
	
	/* load all data into the root node */
	total_trans = loaddata_adult_txt(inputfile);
	DPRINTF(("total trans is %d\n", total_trans));
	
	/* create Decision Tree recusivly */
	gen_decision_tree(root);
	
	return 0;
}
