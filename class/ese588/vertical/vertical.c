/* 
 * ESE588 Project2
 * Vertical Algorithm Implementation
 * 
 * Yufei Ren
 * 108006070
 * yufren@ic.sunysb.edu    
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#include "vertical.h"


void
trans_horizontal_2_vertical(struct transac *t)
{
	int	i;
	int	thisid;
	CANSET	*cp;
	TID	*tp;

	/* for each item, go through the vertical_tqh
	 * if found this item, insert the tid into this canset
	 * else not found, create a new canset, 
	 * 	then insert the id into this canset	 
	 */
	for (i = 0; i < t->num; i ++) {
		cp = NULL;
		thisid = t->itemset[i];
		TAILQ_FOREACH(cp, &vertical_tqh, entries) {
			if (thisid == cp->set[0])
				break;
		}
		
		tp = (TID *) malloc(sizeof(TID));
		memset(tp, '\0', sizeof(TID));
		tp->id = t.id;
		
		if (cp == NULL) {
			cp = (CANSET *) malloc(sizeof(CANSET));
			memset(cp, '\0', sizeof(CANSET));
			TAILQ_INIT(&cp->tidset_tqh);
			
			TAILQ_INSERT_TAIL(&vertical_tqh, cp, entries);
		}
		
		TAILQ_INSERT_TAIL(&cp->tidset_tqh, tp, entries);
		
		cp->count ++;
	}
	
	return;
}


void
find_frequent(int min_sup)
{
	CANSET	*cp, *tmpcp;
	TID	*tp, *tmptp;
	
	/* safe delete */
	for (cp = TAILQ_FIRST(&vertical_tqh); \
		cp != NULL; \
		cp = tmpcp)
	{
		tmpcp = TAILQ_NEXT(cp, entries);
		if (cp->count < min_sup) {
			TAILQ_REMOVE(&vertical_tqh, cp, entries);
			
			for (tp = TAILQ_FIRST(&cp->tidset_tqh); \
				tp != NULL; \
				tp = tmptp)
			{
				tmptp = TAILQ_NEXT(tp, entries);
				TAILQ_REMOVE(&cp->tidset_tqh, tp, entries);
				free(tp);
			}
			
			free(cp);
		} else {
			/* output this candidate set */
		}
	}
	
	return;
}


void
apriori_gen()
{
	CANSET	*csleftp, *csrightp;
	CANSET	*newcp;
	
	csrightp = TAILQ_FIRST(&vertical_tqh);
	for ( csleftp = TAILQ_FIRST(&vertical_tqh); \
		csleftp != NULL; \
		csleftp = TAILQ_NEXT(csleftp, entries) )
	{
		for ( csrightp = TAILQ_FIRST(&vertical_tqh); \
			csrightp != NULL; \
			csrightp = TAILQ_NEXT(csrightp, entries) )
		{
			if ( (newcp = vjoin(csleftp, csrightp)) == NULL)
				continue;
			
			if (has_infrequent_subset(newcp))
				continue;
			
			/* insert this candidate into the temp list */
			
		}
	}

	return;
}


CANSET *
vjoin(CANSET *leftp, CANSET *rightp)
{
	int cap;
	
	/* prefix n-1 are the same && the n-th id is different*/
	if ( (memcmp(leftp->set, rightp->set, (setcap - 1) * sizeof(int)) != 0)
		|| (leftp->set[leftp->setcap-1] == \
			rightp->set[rightp->setcap-1]) )
	{
		return NULL;
	}
	
	newcp = (CANSET *) malloc(sizeof(CANSET));
	memset(newcp, '\0', sizeof(CANSET));
	TAILQ_INIT(&newcp->tidset_tqh);

	cap = leftp->setcap;
	newcp->setcap = cap + 1;
	newcp->set = (int *) malloc(sizeof(int));
	memset(newcp->set, '\0', sizeof(int));
	
	memcpy(newcp->set, leftp->set, (cap - 1) * sizeof(int));
	if (leftp->set[cap - 1] < rightp->set[cap - 1]) {
		newcp->set[cap - 1] = leftp->set[cap - 1];
		newcp->set[cap] = rightp->set[cap - 1];
	} else {
		newcp->set[cap - 1] = rightp->set[cap - 1];
		newcp->set[cap] = leftp->set[cap - 1];
	}
	
	return newcp;
}


int
has_infrequent_subset(CANSET *csp)
{
	return;
}
