#include <sys/queue.h>
#include <stdlib.h>

#ifndef __VERTICAL_H
#define __VERTICAL_H

#define DATA_FIELD_SEP ' '
#define MAX_NUM_PER	10000

struct tid {
	int id;
	TAILQ_ENTRY(tid) entries;	
};
typedef struct tid TID;

struct transac {
	int	id;
	int	num;		/* number of items in this transac */
	int	*itemset;
};

/* candidate set */
struct canset {
	int setcap;		/* number of items in the set */
	int *set;		/* set content */
	int count;		/* # of tid */
	TAILQ_HEAD(, tid)	tidset_tqh;
	
	TAILQ_ENTRY(canset) entries;
};
typedef struct canset CANSET;


TAILQ_HEAD(, canset)	vertical_tqh;
/* join(vertical-left, vertical-right) => canset_tqh */
TAILQ_HEAD(, canset)	canset_tqh;

/* insert one transaction into 
 * the vertical data structure - vertical_tqh
 */
void trans_horizontal_2_vertical(struct transac *t);

/* remove infrequent canset from vertical_tqh */
void find_frequent(int min_sup);

/* generate the candidate from vertical_tqh */
void apriori_gen();

/* return NULL if fail */
CANSET *vjoin(CANSET *leftp, CANSET *rightp);

/* check if this candidate set has infrequent subset
 * 1 - yes
 * 0 - no 
 */
int has_infrequent_subset(CANSET *csp);

void free_canset(CANSET *csp);

/* IO Method */

/* return number of items in the dataset
 * -1:  error
 * >=0: number of items 
 */
int loaddata_txt(char *filepath);


#endif	/* __VERTICAL_H */

