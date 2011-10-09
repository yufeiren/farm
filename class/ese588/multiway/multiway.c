/*
 * ESE588: multiway algorithm implementation
 * Yufei Ren 108006070
 */

#include "multiway.h"

/* update the plate count */
void up_plate(MW_GROUP *group, MW_PLATE *plate)
{
	int i, j;
	MW_GROUP *child_group;
	int tmpdim[MAX_MULTIWAY_DIM];
	
	memset(tmpdim, 0, MAX_MULTIWAY_DIM * sizeof(int));
	memcpy(tmpdim, group->dim, dimnum * sizeof(int));
	tmpdim[plate->starpos] = -1;
	
	for (i = 0; i < plate->unit; i ++) {
		child_group = plate->buffer + i;
		if (memcmp(tmpdim, child_group->dim, dimnum * sizeof(int)) == 0)
		{
			child_group->count += group->count;
			break;
		}
	}
	
	return;
}

void multiway(MW_ITEM *item)
{
	int level;
	int i, j;
	MW_PLATE *plate;
	int tmpdim[MAX_MULTIWAY_DIM];
	prt_dim(item->dim, "multiway this item");

	/* update level(n-1) count */
	plate = NULL;
	TAILQ_FOREACH(plate, &mw_level_tqh[dimnum - 1], entries) {
	  for (i = 0; i < plate->unit; i ++) {
	    memcpy(tmpdim, item->dim, MAX_MULTIWAY_DIM * sizeof(int));

	    /* set the `*' for group by */
	    for (j = 0; j < dimnum; j ++)
	      if (plate->dim[j] == -1)
		tmpdim[j] = -1;

	    if (memcmp(plate->buffer[i].dim, tmpdim, dimnum * sizeof(int)) == 0) {
	      plate->buffer[i].count ++;
	      prt_dim(plate->buffer[i].dim, "group by this cuboids");
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
	plate->chunknum = 1;
	for (i = 0; i < star; i ++) {
		if (plate->dim[i] == -1)
			continue;
		plate->unit *= dimlen[i];
		plate->dimnum[i] = dimlen[i];
		plate->chunknum *= plate->dimnum[i] / chklen[i];
	}

	for (i = star + 1; i < dimnum; i ++) {
		if (plate->dim[i] == -1)
			continue;
		plate->unit *= chklen[i];
		plate->dimnum[i] = chklen[i];
		/* plate->chunknum *= 1; */
	}

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
	
	/* update star column */
	for (j = 0; j < plate->unit; j ++) {
		plate->buffer[j].dim[star] = -1;
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
		memcpy(tmpdim, plate->dim, MAX_MULTIWAY_DIM * sizeof(int));
		tmpdim[i] = -1;
		if (check_plate(plate->level - 1, tmpdim) == 1)
			continue;

		/* create the child node */
		child_plate = (MW_PLATE *) malloc(sizeof(MW_PLATE));
		memset(child_plate, '\0', sizeof(MW_PLATE));
		memcpy(child_plate->dim, tmpdim, \
		       MAX_MULTIWAY_DIM * sizeof(int));

		/* allocate memory for this plate */
		child_plate->level = plate->level - 1;
		group_alloc(i, child_plate);
		child_plate->starpos = i;
		
		printf("plate info\n");
		prt_plate(plate);
		printf("child plate info\n");
		prt_plate(child_plate);

		plate->child[plate->childnum] = child_plate;
		plate->childnum ++;

		/* insert into the level list */
		TAILQ_INSERT_TAIL(&mw_level_tqh[child_plate->level], child_plate, entries);

		build_mmst(child_plate);
	}
	prt_plate(plate);
	printf("has child: %d\n", plate->childnum);

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
		if (dim[i] == -1)
			continue;
		dimbase = chunkbase = 1;
		for (j = 0; j < i; j ++) {
			dimbase *= chknum[j];
			chunkbase *= chklen[j];
		}
		
		*chunkid += (dim[i] / chklen[i]) * dimbase;
		*offset += (dim[i] % chklen[i]) * chunkbase;
	}

	return 0;
}


void
check_aggregate(int chkseq)
{
	int i, j, k;
	int m, n;
	int base;
	int groupbase;

	int tmpdim[MAX_MULTIWAY_DIM];
	MW_PLATE *plate;
	MW_PLATE *child_plate;

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

		prt_plate(plate);

		/* aggregate the child node one by one */
		aggr_child(plate);

		/* write out the plate
		for (k = 0; k < plate->unit; k ++) {
			if (plate->buffer[k].count != 0) {
				prt_dim(plate->buffer[k].dim, \
					"coboid group by result");
				printf("==============> %d\n", \
					plate->buffer[k].count);
			}
		} */


		/* reformat the plate for the next round */
		/* init each group */

		groupbase = 1;
		for (m = 0; m < i; m ++) { /* here i is the star */
			for (n = 0; n < plate->unit; n ++) {
				if (plate->dim[m] == -1)
					plate->buffer[n].dim[m] = -1;
				else
					plate->buffer[n].dim[m] = (n / groupbase) % dimlen[i];
			}
			groupbase *= dimlen[m];
		}

		for (n = 0; n < plate->unit; n ++) {
			plate->buffer[n].dim[i] = -1;
		}

	for (m = i + 1; m < dimnum; m ++) {
	  for (n = 0; n < plate->unit; n ++) {
	    if (plate->dim[m] == -1)
	      plate->buffer[n].dim[m] = -1;
	    else
	      plate->buffer[n].dim[m] = (n / groupbase) % chklen[i];
	  }
	  groupbase *= chklen[m];
	}

	/* reset cuboid's count */
	  for (n = 0; n < plate->unit; n ++) {
		plate->buffer[n].count = 0;
	  }
  }


  return;
}


/* star means aggragate which column */
void
aggr_child(MW_PLATE *plate)
{
	int i, j, k;
	int m, n, p;
	MW_PLATE *child;
	int star;
	int tmpdim[MAX_MULTIWAY_DIM];
	int length;
	int iternum;

	/* check if this plate has child */
	if (plate->childnum == 0) {
		goto PRINT_OUT_PLATE;
	}

	int chunkid, offset;
	int base;
	int seq;
	MW_GROUP* group;

	for (seq = 0; seq < plate->chunknum; seq ++) {
		/* multiway each group */
		for (m = 0; m < plate->unit; m ++) {
			group = plate->buffer + m;
			chunkid = offset = 0;
			cal_chunkid_offset(&chunkid, &offset, group->dim);
			if ( (seq == chunkid)
				&& (group->count != 0) ) {
				/* multiway to child */
printf("chunk id is %d offset %d:  count: %d\n", chunkid, offset, group->count);
				for (n = 0; n < plate->childnum; n ++) {
					child = plate->child[n];
					up_plate(group, child);
				}
			}
		}
		
		for (i = 0; i < dimnum; i ++) {
			if (plate->dim[i] == -1)
				continue;
			
			base = 1;
			
			for (j = 0; j <= i; j ++) {
				base *= chknum[j];
			}
			
			if (seq % base != 0)
				break;
	
			/* update related child, here `i' is the star */
			memset(tmpdim, 0, MAX_MULTIWAY_DIM * sizeof(int));
			memcpy(tmpdim, plate->dim, dimnum * sizeof(int));
			tmpdim[i] = -1;
			
			for (j = 0; j < plate->childnum; j ++) {
				child = plate->child[j];
				if (memcmp(tmpdim, child->dim, dimnum * sizeof(int)) == 0)
					break;
				else
					child = NULL;
			}
			
			if (child != NULL) {
				aggr_child(child);
			}
		}
	}


PRINT_OUT_PLATE:
	for (k = 0; k < plate->unit; k ++) {
		if (plate->buffer[k].count != 0) {
			prt_dim(plate->buffer[k].dim, \
				"aggr_child: coboid group by result");
			printf("==============> %d\n", \
				plate->buffer[k].count);
		}
	}
	
	/* reformat the plate */
	base = 1;
	for (i = 0; i < plate->starpos; i ++) {
		base *= dimlen[i];
		for (j = 0; j < plate->unit; j ++) {
			if (plate->dim[i] == -1)
				plate->buffer[j].dim[i] = -1;
			else
				plate->buffer[j].dim[i] = j / base;
		}
	}
	
	/* update star column */
	for (j = 0; j < plate->unit; j ++) {
		plate->buffer[j].dim[star] = -1;
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


void
multiway_child(MW_GROUP *group, MW_PLATE *plate)
{
	MW_PLATE *child;
	int i, j;
	int tmpdim[MAX_MULTIWAY_DIM];
	
	for (i = 0; i < plate->childnum; i ++) {
		/* update this group into this child */
		child = plate->child[i];
		memcpy(tmpdim, group->dim, dimnum);
		tmpdim[child->starpos] = -1;
		
		for (j = 0; j < child->unit; j ++) {
			if (memcmp(tmpdim, child->buffer[i].dim, dimnum * sizeof(int)) == 0) {
				child->buffer[i].count += group->count;
				break;
			}
		}
	}

	return;
}

void
prt_plate(MW_PLATE *plate)
{
	int i;
	printf("plate: ");
	for (i = 0; i < dimnum; i ++) {
		if (plate->dim[i] == -1)
			printf("*");
		else
			printf("%c", i + 65);
	}
	printf(". level: %d, unit: %d\n", plate->level, plate->unit);
	
	return;
}

void
prt_dim(int *dim, char *msg)
{
	int i;
	printf("item: (");
	for (i = 0; i < dimnum; i ++) {
		if (dim[i] == -1)
			printf("* ");
		else
			printf("%d ", dim[i]);
	}
	printf("): %s\n", msg);
	
	return;
}
