#include <stdio.h>
#include "vertical.h"
#include "iom.h"

/* ./vertical [rawdata] [min_sup_percentage] */

int
main(int argc, char **argv)
{
	char *rawdata, *result;
	int min_sup;
	int total_item;
	
	if (argc != 3) {
		printf("usage: ./%s rawdata [min_sup_percentage]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	totalitems = load_data(argv[1]);
	min_sup = (totalitems / 100) * atoi(argv[2]);
	
	/* construct the level 1 */
	find_frequent(min_sup);
	
	while (!TAILQ_EMPTY(&vertical_tqh)) {
		/* print out the result */
		
		apriori_gen();
		find_frequent(min_sup);
	}
	
	return 0;
}
