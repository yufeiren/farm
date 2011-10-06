/* multiway */

#include <stdio.h>
#include <queue.h>

#define MAX_MULTIWAY_DIM 20

#define F_DEFINATION	"define.txt"
#define F_RAW		"rawdata.txt"
#define F_CHUNK		"chunk.txt"
#define F_RESULT	"result.txt"

#define MW_FIELD_SEP	' '


int dimnum;
int dimlen[MAX_MULTIWAY_DIM];
int chklen[MAX_MULTIWAY_DIM];

int totaltrunknum;

struct Mw_item {
	int chunkid;
	int offset;
	int dim[MAX_MULTIWAY_DIM];
	void *content;

	TAILQ_ENTRY(Mw_item) entries;
};
typedef struct Mw_item MW_ITEM;

struct Mw_plate {
	int count;
	int dim[MAX_MULTIWAY_DIM]; /* dim[x] == -1 means `*' */

	TAILQ_ENTRY(plate) entries;
};
typedef struct Mw_plate MW_PLATE;

TAILQ_HEAD(, Mw_item)		free_mw_item_tqh;
TAILQ_HEAD(, Mw_item)		mw_item_tqh;

TAILQ_HEAD(, Mw_plate)		free_mw_plate_tqh;
TAILQ_HEAD(, Mw_plate)		mw_plate_tqh;

void load_chunk(int chunkid);

void up_plate(MW_ITEM *);

/* process one item, update all related plate */
void multiway(MW_ITEM *);

void raw2chunk(char *raw, char *chunk);

/* parse each row of data, parse into the array */
int parseraw(char *, int *);

int parse_define(void);


int cal_chunkid_offset(int *chunkid, int *offset, int *dim);


