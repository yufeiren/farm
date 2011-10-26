

algorithm
---------
Algorithm: Vertical.

Input:
	D, a database of transaction
	min_sup, the minimum support count threshold.

Output:
	L, frequent itemsets in D

Method:
	D' = trans_horizontal_2_vertical(D)
	L(1) = find_fequent_1-itemsets(D')
	for (k = 0; L(k-1) != NULL; k ++) {
		Candidate(k) = apriori_gen(L(k-1))
		for each pattern p in Candidate(k)
			if p.count < min_sup
				delete p;
	}
	return Union(Candidate(k))

Procedure apriori_gen( L(k-1) )
	for each pattern pLeft in L(k-1)
		for each pattern pRight in L(k-1)
			p = join(pLeft, pRight)
			if has_infrequent_subset(p, L(k-1)) then
				delete p;
			else
				add p to Ck
	return Ck

Procedure has_infrequent_subset(p: candidate, L(k-1))
	// use prior knowledge
	for each (k-1) subset s of p
		if s not belongs to L(k-1)
			return True
	return False

Procedure trans_horizontal_2_vertical(D)
	for each transaction t of D
		update each item of T into D'

notes
-----
1. reduce the cost of registering long TID_sets: diffset

2. if some purchase occures twice, how to handle that?
