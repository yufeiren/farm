/* multiway */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#define MAX_MULTIWAY_DIM 20

#define F_DEFINATION	"sample/define.txt"
#define F_RAW		"sample/rawdata.txt"
#define F_CHUNK		"sample/chunk.txt"
#define F_RESULT	"sample/result.txt"

#define MW_FIELD_SEP	' '


int dimnum;
int dimlen[MAX_MULTIWAY_DIM];
int chklen[MAX_MULTIWAY_DIM];
int chknum[MAX_MULTIWAY_DIM];

int totaltrunknum;

struct Mw_item {
	int chunkid;
	int offset;
	int dim[MAX_MULTIWAY_DIM];
	void *content;

	TAILQ_ENTRY(Mw_item) entries;
};
typedef struct Mw_item MW_ITEM;

struct Mw_group {
  int dim[MAX_MULTIWAY_DIM];
  int count;
};
typedef struct Mw_group MW_GROUP;

struct Mw_plate {
	int unit;
	int level;
	int dim[MAX_MULTIWAY_DIM]; /* dim[x] == -1 means `*' */
	MW_GROUP *buffer;
};
typedef struct Mw_plate MW_PLATE;

struct Mw_level {
	int level;
	TAILQ_HEAD(, Mw_plate) plate;
};
typedef struct Mw_level MW_LEVEL;

TAILQ_HEAD(, Mw_item)		free_mw_item_tqh;
TAILQ_HEAD(, Mw_item)		mw_item_tqh;

TAILQ_HEAD(, Mw_plate)		free_mw_plate_tqh;
TAILQ_HEAD(, Mw_plate)		mw_plate_tqh;

void group_alloc(int star, MW_PLATE *plate);

void build_mmst(MW_PLATE *plate);

void load_chunk(int chunkid);

void up_plate(MW_ITEM *);

/* process one item, update all related plate */
void multiway(MW_ITEM *);

void raw2chunk(char *raw, char *chunk);

/* parse each row of data, parse into the array */
int parseraw(char *, int *);

int parse_define(void);


int cal_chunkid_offset(int *chunkid, int *offset, int *dim);


