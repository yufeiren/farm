/*
 * ESE588: multiway algorithm implementation
 * Yufei Ren 108006070
 */

#include "multiway.h"

/* update the plate count */
void up_plate(MW_ITEM *item)
{
	


  return;
}

void multiway(MW_ITEM *item)
{
	
  return;
}


void
load_chunk(int chunkid)
{

	int tmpchunkid;
	int tmpoffset;
	MW_ITEM *item;

	char *start;
	char *end;
	char buf[32];

	FILE *fp = fopen(F_CHUNK, "r");
	if (fp == NULL) {
		fprintf(stderr, "can not open chunk file: %s\n", F_CHUNK);
		exit(EXIT_FAILURE);
	}

  /* take an item from free_mw_item list */
	while (fgets(line, 1024, fp) != NULL) {
	  /* chunkid offset */
		memset(buf, '\0', 32);
		start = line;
		end = strchr(start, MW_FIELD_SEP);
		memcpy(buf, start, end - start);
		tmpchunkid = atoi(buf);

		if (tmpchunkid != chunkid)
			continue;

		memset(buf, '\0', 32);
		start = end + 1;
		end = strchr(start, MW_FIELD_SEP);
		memcpy(buf, start, end - start);
		tmpoffset = atoi(buf);

		/* if id is the chunk id, insert this line into the mw_item list */
		if (TAILQ_EMPTY(&free_mw_item_tqh)) {
			fprintf(stderr, "no free item\n");
			exit(EXIT_FAILURE);
		}

		item = TAILQ_FIRST(&free_mw_item_tqh);
		TAILQ_REMOVE(&free_mw_item_tqh, item, entries);

		/* setup content */
		item->chunkid = tmpchunkid;
		item->offset = tmpoffset;

		start = end + 1;
		parseline(start, item->dim);

		TAILQ_INSERT_TAIL(&mw_item_tqh, item, entries);	
	}

	return;
}

void
raw2chunk(char *raw, char *chunk)
{
	FILE *rf, *cf;
	int dim[MAX_MULTIWAY_DIM];
	int chunkid;
	int offset;

	rf = fopen(raw, "r");
	if (rf == NULL) {
		fprintf(stderr, "can not open file: %s\n", raw);
		exit(EXIT_FAILURE);
	}
	
	cf = fopen(chunk, "w");
	if (cf == NULL) {
		fprintf(stderr, "can not open file: %s\n", chunk);
		exit(EXIT_FAILURE);
	}

	while (fgets(line, 1024, rf) != NULL) {
		parseline(line, dim);

		cal_chunkid_offset(&chunkid, &offset, dim);

		fprintf(cf, "%d %d ", chunkid, offset);

		for (i = 0; i < dimnum; i ++) {
			fprintf(cf, "%d ", dim[i]);
		}
		fprintf(cf, "\n");
	}

	return;
}

int parseline(char *buf, int *array)
{
  char *start;
  char *end;
  char value[16];
  start = buf;
  int i;

  for (i = 0; (end = strchr(start, MW_FIELD_SEP)) != NULL; i++) {
		memset(value, '\0', 16);
		memcpy(value, start, end - start);
		*(array + i) = atoi(value);

		start = end + 1;
  }

  return 0;
}


int parse_define(void)
{
  char line[1024];
  FILE *fp;
	fp = fopen(F_DEFINATION, "r");
	if (fp == NULL) {
		fprintf(stderr, "can not open file: %s\n", );
		exit(EXIT_FAILURE);
	}
	
	/* number of dimension */
	memset(line, '\0', 1024);
	if (fgets(line, 1024, fp) != NULL)
	  parseline(line, &dimnum);

	/* dimensions */
	memset(line, '\0', 1024);
	if (fgets(line, 1024, fp) != NULL)
	  parseline(line, dimlen);

	/* chunks */
	memset(line, '\0', 1024);
	if (fgets(line, 1024, fp) != NULL)
		parseline(line, chklen);

	fclose(fp);

  return 0;
}

int
cal_chunkid_offset(int *chunkid, int *offset, int *dim)
{
	int i, j;
	int tmp;
	
	for (i = 0; i < dimnum; i ++) {
		for (j = 0; j < i; j ++)
			tmp *= dimlen[j];
		
		*chunkid += (dim[i] / chklen[i]) * tmp;
		*offset += (dim[i] % chklen[i]) * tmp;
	}

	printf("chunk id is %d\n", chunkid);
	printf("offset   is %d\n", offset);

	return 0;
}

