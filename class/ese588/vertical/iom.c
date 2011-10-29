#include <stdio.h>
#include <errno.h>
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
	while (fgets(line, 10240, fp) != NULL) {
		memset(buf, '\0', sizeof(buf));
		start = line;
		end = strchr(start, DATA_FIELD_SEP);
		memcpy(buf, start, end - start);
		t.id = atoi(buf);

		for (t.num = 0; (start = end + 1) != NULL; t.num ++) {
			if ((end = strchr(start, DATA_FIELD_SEP)) == NULL)
				break;
			
			memset(buf, '\0', sizeof(buf));
			memcpy(buf, start, end - start);
			t.itemset[t.num] = atoi(buf);
		}
		
		trans_horizontal_2_vertical(&t);

		memset(line, '\0', sizeof(line));
	}

	fclose(cf);
	
	return;
}
