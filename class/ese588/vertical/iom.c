#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "vertical.h"

int
loaddata_txt(char *filepath)
{
	if (filepath == NULL)
		return -1;
	
	FILE *fp;
	char line[10240];
	char *start;
	char *end;
	char buf[64];
	int total_trans;
	
	fp = fopen(filepath, "r");
	if (fp == NULL) {
		fprintf(stderr, "can not open chunk file: %s\n", filepath);
		return -1;
	}

	struct transac t;
	t.itemset = (int *) malloc(MAX_NUM_PER * sizeof(int));
	if (t.itemset == NULL) {
		fprintf(stderr, "malloc fail: (%d)%s\n", \
			errno, strerror(errno));
		return -1;
	}
	
	/* TransactionID Item1 Item2 ... ItemN */
	memset(line, '\0', sizeof(line));
	total_trans = 0;
	while (fgets(line, 10240, fp) != NULL) {
DPRINTF(("parse line: %s\n", line));
		total_trans ++;
		memset(buf, '\0', sizeof(buf));
		start = line;
		end = strchr(start, DATA_FIELD_SEP);
		memcpy(buf, start, end - start);
		t.id = atoi(buf);
DPRINTF(("transac id: %d\n", t.id));

		for (t.num = 0; (start = end + 1) != NULL; t.num ++) {
			if ((end = strchr(start, DATA_FIELD_SEP)) == NULL)
				break;
			
			memset(buf, '\0', sizeof(buf));
			memcpy(buf, start, end - start);
			t.itemset[t.num] = atoi(buf);
DPRINTF(("item id: %d\n", t.itemset[t.num]));
		}
DPRINTF(("transac item num: %d\n", t.num));
		
		trans_horizontal_2_vertical(&t);

		memset(line, '\0', sizeof(line));
	}

	fclose(fp);
	
	return total_trans;
}

void
prt_canset(CANSET *csp, char *msg)
{
	/* output:
	 * (item set): transaction list
	 */
	int i;
	TID *t;
	
	if (csp == NULL)
		return;
	
	printf("%s: (", msg);
	for (i = 0; i < csp->setcap; i ++) {
		printf("%d,", csp->set[i]);
	}
	printf("): ");
	
	TAILQ_FOREACH(t, &csp->tidset_tqh, entries) {
		printf("%d ", t->id);
	}
	
	printf("\n");
}
