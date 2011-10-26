#include <sys/queue.h>

#ifndef __VERTICAL_H
#define __VERTICAL_H

struct tid {
	int id;
	TAILQ_ENTRY(tids) entries;	
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

#endif	/* __VERTICAL_H */

