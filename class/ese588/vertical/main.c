#include <stdio.h>
#include "vertical.h"

/* ./vertical [rawdata] [min_sup_percentage] */

int
main(int argc, char **argv)
{
	char *rawdata, *result;
	int min_sup;
	int total_trans;
	
	if (argc != 3) {
		printf("usage: ./%s rawdata [min_sup_percentage]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	TAILQ_INIT(&vertical_tqh);
	TAILQ_INIT(&canset_tqh);

	total_trans = loaddata_txt(argv[1]);
	DPRINTF(("total trans is %d\n", total_trans));
	
	min_sup = (total_trans / 100) * atoi(argv[2]);
	min_sup = 1;
	/* construct the level 1 */
	find_frequent(min_sup);
	
	while (!TAILQ_EMPTY(&vertical_tqh)) {
		/* print out the result */
		
		apriori_gen();
		find_frequent(min_sup);
	}
	
	return 0;
}
