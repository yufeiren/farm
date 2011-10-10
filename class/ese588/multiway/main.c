#include <stdio.h>
#include "multiway.h"

int debug = 0;

int
main(int argc, char **argv)
{
	int i;

	for (i = 0; i < MAX_MULTIWAY_DIM; i ++) {
		TAILQ_INIT(&mw_level_tqh[i]);
	}

	TAILQ_INIT(&free_mw_item_tqh);
	TAILQ_INIT(&mw_item_tqh);

	if ((argc == 2) && (memcmp(argv[1], "-D", 2) == 0))
		debug = 1;

	/* parse define.txt */
	parse_define();

	/* parse raw data to trunk data */
	raw2chunk(F_RAW, F_CHUNK);

	/* load chunk one by one */
	totaltrunknum = 1;
	for (i = 0; i < dimnum; i ++) {
		totaltrunknum *= chknum[i];
	}
	
	if (debug)
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
	if (debug)
		printf("start building the MMST\n");
	build_mmst(&plate);
	if (debug)
		printf("finish building the MMST\n");

	for (i = 0; i < totaltrunknum; i ++) {
		/* check if the some plate needs to write out
		 * and reformat
		 */
	 	check_aggregate(i);
	 	
	 	if (debug)
			printf("start load chunk %d\n", i);
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
