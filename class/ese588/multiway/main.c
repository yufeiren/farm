#include <stdio.h>
#include "multiway.h"

int
main(int argc, char **argv)
{
  int i, j;
  int tmpdim[MAX_MULTIWAY_DIM];

	for (i = 0; i < MAX_MULTIWAY_DIM; i ++) {
	  TAILQ_INIT(&mw_level_tqh[i]);
	}

	TAILQ_INIT(&free_mw_item_tqh);
	TAILQ_INIT(&mw_item_tqh);

	/* parse define.txt */
	parse_define();

	/* parse raw data to trunk data */
	raw2chunk(F_RAW, F_CHUNK);

	/* load chunk one by one */
	totaltrunknum = 1;
	for (i = 0; i < dimnum; i ++) {
		totaltrunknum *= chknum[i];
	}
	printf("chunk number is %d\n", totaltrunknum);

	MW_ITEM *item;
	for (i = 0; i < totaltrunknum; i ++) {
		item = (MW_ITEM *) malloc(sizeof(MW_ITEM));
		memset(item, '\0', sizeof(MW_ITEM));
		TAILQ_INSERT_TAIL(&free_mw_item_tqh, item, entries);
	}

	MW_PLATE plate;
	memset(&plate, '\0', sizeof(MW_PLATE));
	plate.level = dimnum;
	printf("start building the MMST\n");
	build_mmst(&plate);
	printf("finish building the MMST\n");

	MW_PLATE *child;
	for (i = 0; i < totaltrunknum; i ++) {
		/* check if the some plate needs to write out
		 * and reformat
		 */
	  int base;

	  for (j = 0; j < dimnum; j ++) {
	    base = 1;
	    child = NULL;

	    for (k = 0; k <= i; k ++) {
	      base *= chknum[k];
	    }

	    if (chkseq % base != 0)
	      break;

		memset(tmpdim, 0, MAX_MULTIWAY_DIM * sizeof(int));
		tmpdim[j] = -1;

		TAILQ_FOREACH(child, &mw_level_tqh[dimnum - 1], entries) {
			if (memcmp(tmpdim, child->dim, dimnum * sizeof(int)) == 0)
				break;
		}

		prt_plate(child);

		aggr_child(child);

		dump_plate(child);

	  }
	  /*		check_aggregate(i); */

		printf("\n start load chunk %d\n", i);
		load_chunk(i);
		/* multiway each chunk */
		while (!TAILQ_EMPTY(&mw_item_tqh)) {
			item = TAILQ_FIRST(&mw_item_tqh);
			TAILQ_REMOVE(&mw_item_tqh, item, entries);

			multiway(item);
			TAILQ_INSERT_TAIL(&free_mw_item_tqh, item, entries);
		}
	}

	/* load the pseudo last chunk */
	check_aggregate(i);

	exit(EXIT_SUCCESS);
}
