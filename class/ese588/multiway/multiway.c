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
	int i;
	for (i = 0; i < dimnum; i ++) {
	  printf("%d ", item->dim[i]);
	}
	printf("\n");
		
	return;
}

/* star means the '*' in sql search */
void
group_alloc(int star, MW_PLATE *plate)
{
	int i, j;

	plate->unit = 1;
	for (i = 0; i < star; i ++) {
		if (plate->dim[i] == -1)
			continue;
		plate->unit *= dimlen[i];
	}

	for (i = star + 1; i < dimnum; i ++) {
		if (plate->dim[i] == -1)
			continue;
		plate->unit *= chklen[i];
	}

	printf("total unit is %d\n", plate->unit);

	plate->buffer = (MW_GROUP *) malloc(plate->unit * sizeof(MW_GROUP));
	memset(plate->buffer, '\0', plate->unit * sizeof(MW_GROUP));

	/* init each group */
	for (i = 0; i < star; i ++) {
	  for (j = 0; j < plate->unit; j ++) {
	    if (plate->dim[i] == -1)
	      plate->buffer[j].dim[i] = -1;
	    else
	      plate->buffer[j].dim[i] = j % dimlen[i];
	  }
	}

	for (i = star + 1; i < dimnum; i ++) {
	  for (j = 0; j < plate->unit; j ++) {
	    if (plate->dim[i] == -1)
	      plate->buffer[j].dim[i] = -1;
	    else
	      plate->buffer[j].dim[i] = j % chklen[i];
	  }
	}

	return;
}

void build_mmst(MW_PLATE *plate)
{
	if (plate->level == 1)
		return;

	int i;
	int tmpdim[MAX_MULTIWAY_DIM];

	MW_PLATE *child_plate;
	for (i = 0; i < dimnum; i ++) {
		if (plate->dim[i] == -1)
			continue;

		/* check if the child node exist */
		memcpy(tmpdim, plate->dim, MAX_MULTIWAY_DIM*sizeof(int));
		tmpdim[i] = -1;

		/* create the child node */
		child_plate = (MW_PLATE *) malloc(sizeof(MW_PLATE));
		memset(child_plate, '\0', sizeof(MW_PLATE));
		memcpy(child_plate->dim, tmpdim, \
		       MAX_MULTIWAY_DIM * sizeof(int));

		/* allocate memory for this plate */
		child_plate->level = plate->level - 1;
		group_alloc(i, child_plate);

		build_mmst(child_plate);
	}

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
	char line[1024];

	FILE *cf = fopen(F_CHUNK, "r");
	if (cf == NULL) {
		fprintf(stderr, "can not open chunk file: %s\n", F_CHUNK);
		exit(EXIT_FAILURE);
	}

  /* take an item from free_mw_item list */
	memset(line, '\0', 1024);
	while (fgets(line, 1024, cf) != NULL) {
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

		memset(line, '\0', 1024);
	}

	fclose(cf);

	return;
}

void
raw2chunk(char *raw, char *chunk)
{
	FILE *rf, *cf;
	int dim[MAX_MULTIWAY_DIM];
	int chunkid;
	int offset;
	char line[1024];
	int i;

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

		chunkid = offset = 0;
		cal_chunkid_offset(&chunkid, &offset, dim);

		fprintf(cf, "%d %d ", chunkid, offset);

		for (i = 0; i < dimnum; i ++) {
			fprintf(cf, "%d ", dim[i]);
		}
		fprintf(cf, "\n");
	}

	fclose(rf);
	fclose(cf);

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


int parse_define()
{
  char line[1024];
  FILE *fp;
	fp = fopen(F_DEFINATION, "r");
	if (fp == NULL) {
		fprintf(stderr, "can not open file: %s\n", F_DEFINATION);
		exit(EXIT_FAILURE);
	}
	
	/* number of dimension */
	memset(line, '\0', 1024);
	if (fgets(line, 1024, fp) != NULL)
		parseline(line, &dimnum);
	printf("dimnum: %d\n", dimnum);

	/* dimensions */
	memset(line, '\0', 1024);
	if (fgets(line, 1024, fp) != NULL)
		parseline(line, dimlen);

	/* chunks */
	memset(line, '\0', 1024);
	if (fgets(line, 1024, fp) != NULL)
		parseline(line, chklen);

	/* cal number of chunks in each dimension */
	int i;
	for (i = 0; i < dimnum; i ++) {
		if (dimlen[i] % chklen[i] == 0)
			chknum[i] = dimlen[i] / chklen[i];
		else
			chknum[i] = (dimlen[i] / chklen[i] + 1);
		printf("dim-%d: number of chunks %d\n", i, chknum[i]);
	}

	fclose(fp);

  return 0;
}

int
cal_chunkid_offset(int *chunkid, int *offset, int *dim)
{
	int i, j;
	int dimbase;
	int chunkbase;
	
	for (i = 0; i < dimnum; i ++) {
		dimbase = chunkbase = 1;
		for (j = 0; j < i; j ++) {
			dimbase *= chknum[j];
			chunkbase *= chklen[j];
		}
		
		*chunkid += (dim[i] / chklen[i]) * dimbase;
		*offset += (dim[i] % chklen[i]) * chunkbase;
	}
	printf("chunk id is %d\n", *chunkid);
	printf("offset   is %d\n", *offset);

	return 0;
}

