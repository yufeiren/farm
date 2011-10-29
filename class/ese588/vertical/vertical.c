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
	
	CANSET	*pre;
	
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
		tp->id = t->id;
		
		if (cp == NULL) {
			cp = (CANSET *) malloc(sizeof(CANSET));
			memset(cp, '\0', sizeof(CANSET));
			
			cp->set = (int *) malloc(sizeof(int));
			cp->set[0] = thisid;
			cp->setcap = 1;
			
			TAILQ_INIT(&cp->tidset_tqh);
			
			TAILQ_FOREACH(pre, &vertical_tqh, entries) {
                                if (thisid < pre->set[0]) {
                                        TAILQ_INSERT_BEFORE(pre, cp, entries);
                                        break;
                                }
                        }
                        if (pre == NULL)
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
			prt_canset(cp, "frequent pattern");
		}
	}
	
	return;
}


void
apriori_gen()
{
	CANSET	*csleftp, *csrightp, *tmpcsp;
	CANSET	*newcp;
	
	TAILQ_FOREACH(csleftp, &vertical_tqh, entries) {
		TAILQ_FOREACH(csrightp, &vertical_tqh, entries) {
			if ( (newcp = vjoin(csleftp, csrightp)) == NULL)
				continue;
			
			if (has_infrequent_subset(newcp)) {
				/* free all the nodes in the candidate set */
				free_canset(newcp);
				continue;
			}
			
			/* insert this candidate into the canset_tqh */
			TAILQ_INSERT_TAIL(&canset_tqh, newcp, entries);
		}
	}
	
	/* free all the nodes in the vertical_tqh */
	for ( csleftp = TAILQ_FIRST(&vertical_tqh); \
		csleftp != NULL; \
		csleftp = tmpcsp )
	{
		tmpcsp = TAILQ_NEXT(csleftp, entries);
		TAILQ_REMOVE(&vertical_tqh, csleftp, entries);
		free_canset(csleftp);
	}
	
	/* insert all the nodes from canset to vertical */
	for ( csleftp = TAILQ_FIRST(&canset_tqh); \
		csleftp != NULL; \
		csleftp = tmpcsp )
	{
		tmpcsp = TAILQ_NEXT(csleftp, entries);
		
		TAILQ_REMOVE(&canset_tqh, csleftp, entries);
		
		TAILQ_INSERT_TAIL(&vertical_tqh, csleftp, entries);
	}

	return;
}


CANSET *
vjoin(CANSET *leftp, CANSET *rightp)
{
	int cap;
	CANSET *newcp;
	TID *lt, *rt, *newt;
	cap = leftp->setcap;
	
	/* prefix n-1 are the same && the left n-th id is smaller */
	if ( (memcmp(leftp->set, rightp->set, (cap - 1) * sizeof(int)) != 0)
		|| !(leftp->set[cap - 1] < rightp->set[cap - 1]) )
	{
		return NULL;
	}
	
	newcp = (CANSET *) malloc(sizeof(CANSET));
	memset(newcp, '\0', sizeof(CANSET));
	TAILQ_INIT(&newcp->tidset_tqh);

	newcp->setcap = cap + 1;
	newcp->set = (int *) malloc(newcp->setcap * sizeof(int));
	memset(newcp->set, '\0', newcp->setcap * sizeof(int));
	
	memcpy(newcp->set, leftp->set, (cap - 1) * sizeof(int));
	if (leftp->set[cap - 1] < rightp->set[cap - 1]) {
		newcp->set[cap - 1] = leftp->set[cap - 1];
		newcp->set[cap] = rightp->set[cap - 1];
	} else {
		newcp->set[cap - 1] = rightp->set[cap - 1];
		newcp->set[cap] = leftp->set[cap - 1];
	}
	
	/* merge tid set */
	TAILQ_FOREACH(lt, &leftp->tidset_tqh, entries) {
		TAILQ_FOREACH(rt, &rightp->tidset_tqh, entries) {
			if (lt->id == rt->id) {
				newt = (TID *) malloc(sizeof(TID));
				newt->id = lt->id;
				TAILQ_INSERT_TAIL(&newcp->tidset_tqh, newt, entries);
				newcp->count ++;
			}
		}
	}
	
	return newcp;
}


int
has_infrequent_subset(CANSET *csp)
{
	int i;
	int *set;
	CANSET	*cp;
	
	set = (int *) malloc((csp->setcap - 1) * sizeof(int));
	for (i = 0; i < csp->setcap; i ++) {
		memset(set, '\0', (csp->setcap - 1) * sizeof(int));
		
		/* prefix */
		memcpy(set, csp->set, i * sizeof(int));
		/* suffix */
		memcpy(set + i, csp->set + i + 1, \
			(csp->setcap - i - 1) * sizeof(int));
		
		/* check whether this is contained in the prior knowledge */
		TAILQ_FOREACH(cp, &vertical_tqh, entries) {
			if (memcmp(set, cp->set, (csp->setcap - 1) * sizeof(int)) == 0)
				break;
		}
		
		if (cp == NULL) {
			free(set);
			return 1;
		}
	}
	
	free(set);
	return 0;
}


void
free_canset(CANSET *csp)
{
	TID *tidp, *tmptidp;
	
	if (csp == NULL)
		return;
	
	for ( tidp = TAILQ_FIRST(&csp->tidset_tqh); \
		tidp != NULL; \
		tidp = TAILQ_NEXT(tmptidp, entries) )
	{
		tmptidp = tidp;
		
		free(tidp);
	}
	
	free(csp);
	
	return;
}
