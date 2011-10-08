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
	int level;
	int i, j;
	MW_PLATE *plate;
	int tmpdim[MAX_MULTIWAY_DIM];

	/* update level(n-1) count */
	plate = NULL;
	TAILQ_FOREACH(plate, &mw_level_tqh[dimnum - 1], entries) {
	  for (i = 0; i < plate->unit; i ++) {
	    memcpy(tmpdim, item->dim, MAX_MULTIWAY_DIM * sizeof(int));
	    /* set the `*' for group by */
	    for (j = 0; j < dimnum; j ++)
	      if (plate->dim[j] == -1)
		tmpdim[j] = -1;
	    if (memcmp(plate->buffer[i].dim, item->dim, dimnum * sizeof(int)) == 0) {
	      plate->buffer[i].count ++;
	      break;
	    }
	  }
	}

	return;
}

/* star means the '*' in sql search */
void
group_alloc(int star, MW_PLATE *plate)
{
	int i, j;
	int base;

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

	printf("level: %d, plate unit %d\n", plate->level, plate->unit);

	plate->buffer = (MW_GROUP *) malloc(plate->unit * sizeof(MW_GROUP));
	memset(plate->buffer, '\0', plate->unit * sizeof(MW_GROUP));

	/* init each group */
	base = 1;
	for (i = 0; i < star; i ++) {
	  base *= dimlen[i];
	  for (j = 0; j < plate->unit; j ++) {
	    if (plate->dim[i] == -1)
	      plate->buffer[j].dim[i] = -1;
	    else
	      plate->buffer[j].dim[i] = j / base;
	  }
	}

	for (i = star + 1; i < dimnum; i ++) {
	  base *= chklen[i];
	  for (j = 0; j < plate->unit; j ++) {
	    if (plate->dim[i] == -1)
	      plate->buffer[j].dim[i] = -1;
	    else
	      plate->buffer[j].dim[i] = j / chklen[i];
	  }
	}

	return;
}


int
check_plate(int level, int *dim)
{
  MW_PLATE *plate = NULL;
  TAILQ_FOREACH(plate, &mw_level_tqh[level], entries)
    if (memcmp(dim, plate->dim, sizeof(int) * MAX_MULTIWAY_DIM) == 0)
      return 1;

  return 0;
}

void
build_mmst(MW_PLATE *plate)
{
	if (plate->level == 0)
		return;

	int i;
	int tmpdim[MAX_MULTIWAY_DIM];

	MW_PLATE *child_plate;
	/* i is the coordinate of the dimension */
       	for (i = dimnum - 1; i > -1; i --) {
		if (plate->dim[i] == -1)
			continue;

		/* check if the child node exist */
		memcpy(tmpdim, plate->dim, MAX_MULTIWAY_DIM*sizeof(int));
		tmpdim[i] = -1;
		if (check_plate(plate->level - 1, tmpdim) == 1)
			continue;

		/* create the child node */
		child_plate = (MW_PLATE *) malloc(sizeof(MW_PLATE));
		memset(child_plate, '\0', sizeof(MW_PLATE));
		memcpy(child_plate->dim, tmpdim, \
		       MAX_MULTIWAY_DIM * sizeof(int));

		plate->child[dimnum - (i + 1)] = child_plate;

		/* allocate memory for this plate */
		child_plate->level = plate->level - 1;
		group_alloc(i, child_plate);

		/* insert into the level list */
		TAILQ_INSERT_TAIL(&mw_level_tqh[child_plate->level], child_plate, entries);

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


void
check_aggregate(int chkseq)
{
	int i, j, k;
	int base;
	int groupbase;

	int tmpdim[MAX_MULTIWAY_DIM];
	MW_PLATE *plate;

	for(i = 0; i < dimnum; i ++) {
    base = 1;
    plate = NULL;

    for (j = 0; j <= i; j ++) {
      base *= chknum[j];
    }

    if (chkseq % base != 0)
      break;

      /* 0. find out related plate
       * 1. update child
       * 2. write out the plate and/or child plate
       * 3. reformat the plate
       */
      memset(tmpdim, 0, MAX_MULTIWAY_DIM * sizeof(int));
      tmpdim[i] = -1;

      TAILQ_FOREACH(plate, &mw_level_tqh[dimnum - 1], entries) {
	if (memcmp(tmpdim, plate->dim, dimnum * sizeof(int)) == 0)
	  break;
      }

      /* aggregate child node */

      /* write out the plate */
      for (k = 0; k < plate->unit; k ++) {
	if (plate->buffer[k].count != 0)
	  printf("group by: %d\n", plate->buffer[k].count);
      }

      /* reformat the plate for the next round */
	/* init each group */
      int m, n;
	groupbase = 1;
	for (m = 0; m < i; m ++) { /* here i is the star */
	  groupbase *= dimlen[m];
	  for (n = 0; n < plate->unit; n ++) {
	    if (plate->dim[m] == -1)
	      plate->buffer[n].dim[m] = -1;
	    else
	      plate->buffer[n].dim[m] = n / groupbase;
	  }
	}

	for (m = i + 1; m < dimnum; m ++) {
	  groupbase *= chklen[m];
	  for (n = 0; n < plate->unit; n ++) {
	    if (plate->dim[m] == -1)
	      plate->buffer[n].dim[m] = -1;
	    else
	      plate->buffer[n].dim[m] = n / chklen[m];
	  }
	}
  }

  return;
}

