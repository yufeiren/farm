#include "dt.h"

int
tuples_same_class(Dt_node *dtn)
{
	Adult_rec *ap;
	int income_class;
	
	if (TAILQ_EMPTY(&dtn->adult_rec_tqh))
		return 1;
	
	ap = TAILQ_FIRST(&dtn->adult_rec_tqh);
	income_class = ap->income_class;
	
	TAILQ_FOREACH(ap, &dtn->adult_rec_tqh, entries) {
		if (income_class != ap->income_class)
			return 0;
	}
	
	return income_class;
}

int
get_attribute_num(Attr_list *alist)
{
	int i;
	int count;
	
	count = 0;
	for (i = 0; i < ADULT_ATTRIBUTE_NUM; i ++) {
		if (alist->attr_is_valid[i] == 1)
			count ++;
	}
	
	return count;
}

int
get_majority_class(Dt_node *dtn)
{
	int i;
	int count;
	Adult_rec *ap;
	
	int poor_num;
	int rich_num;
	
	poor_num = rich_num = 0;
	TAILQ_FOREACH(ap, &dtn->adult_rec_tqh, entries) {
		if (ap->income_class == 1)
			poor_num ++;
		else
			rich_num ++;
	}
	
	if (poor_num > rich_num)
		return 1;	/* <= 50K */
	else
		return 2;	/* > 50K */
}

int
cal_info_gain(Dt_node *dtn)
{
	int i, j, k;
	int count;
	Adult_rec *ap;
	double info_whole;
	
	int poor_num;
	int rich_num;
	
	poor_num = rich_num = 0;
	TAILQ_FOREACH(ap, &dtn->adult_rec_tqh, entries) {
		if (ap->income_class == 1) {
			poor_num ++;
			for (i = 0; i < ADULT_ATTRIBUTE_NUM; i ++) {
				dtn->node_attr_list.poor_num[i] ++;
				switch (i) {
				case 0:
					dtn->node_attr_list.poor_num_dist[0][ap->workclass] ++;
					break;
				case 1:
					dtn->node_attr_list.poor_num_dist[1][ap->education] ++;
					break;
				case 2:
					dtn->node_attr_list.poor_num_dist[2][ap->marital_status] ++;
					break;
				case 3:
					dtn->node_attr_list.poor_num_dist[3][ap->occupation] ++;
					break;
				case 4:
					dtn->node_attr_list.poor_num_dist[4][ap->relationship] ++;
					break;
				case 5:
					dtn->node_attr_list.poor_num_dist[5][ap->race] ++;
					break;
				case 6:
					dtn->node_attr_list.poor_num_dist[6][ap->sex] ++;
					break;
				case 7:
					dtn->node_attr_list.poor_num_dist[7][ap->native_country] ++;
					break;
				default:
					break;
				}
			}
		} else {
			rich_num ++;
			for (i = 0; i < ADULT_ATTRIBUTE_NUM; i ++) {
				dtn->node_attr_list.rich_num[i] ++;
				switch (i) {
				case 0:
					dtn->node_attr_list.rich_num_dist[0][ap->workclass] ++;
					break;
				case 1:
					dtn->node_attr_list.rich_num_dist[1][ap->education] ++;
					break;
				case 2:
					dtn->node_attr_list.rich_num_dist[2][ap->marital_status] ++;
					break;
				case 3:
					dtn->node_attr_list.rich_num_dist[3][ap->occupation] ++;
					break;
				case 4:
					dtn->node_attr_list.rich_num_dist[4][ap->relationship] ++;
					break;
				case 5:
					dtn->node_attr_list.rich_num_dist[5][ap->race] ++;
					break;
				case 6:
					dtn->node_attr_list.rich_num_dist[6][ap->sex] ++;
					break;
				case 7:
					dtn->node_attr_list.rich_num_dist[7][ap->native_country] ++;
					break;
				default:
					break;
				}
			}
		}
		
	}
	
	count = poor_num + rich_num;
	
	double p_poor = (((double) poor_num) / ((double) count));
	double p_rich = (((double) rich_num) / ((double) count));
	
	info_whole = ((double) -1) * (p_poor * log2(p_poor) + p_rich * log2(p_rich));
	
	for (i = 0; i < ADULT_ATTRIBUTE_NUM; i ++) {
		if (dtn->node_attr_list.attr_is_valid[i] == 0)
			continue;
		
		for (j = 0; j < attr_range[i]; j ++) {
			p_poor = ((double) (dtn->node_attr_list.poor_num_dist[i][j])) / ((double) (dtn->node_attr_list.poor_num_dist[i][j] + dtn->node_attr_list.rich_num_dist[i][j]));
			p_rich = ((double) (dtn->node_attr_list.rich_num_dist[i][j])) / ((double) (dtn->node_attr_list.poor_num_dist[i][j] + dtn->node_attr_list.rich_num_dist[i][j]));
			dtn->node_attr_list.info[i] += ((double) (dtn->node_attr_list.poor_num_dist[i][j] + dtn->node_attr_list.rich_num_dist[i][j]) / ((double) count) ) * ((double) -1) * (p_poor * log2(p_poor) + p_rich * log2(p_rich));
		}
		
		dtn->node_attr_list.gain[i] = info_whole - dtn->node_attr_list.info[i];
	}
	
	/* get the highest information gain */
	int index;
	double max_gain = 0;
	for (i = 0; i < ADULT_ATTRIBUTE_NUM; i ++) {
		if (dtn->node_attr_list.gain[i] > max_gain) {
			index = i;
			max_gain = dtn->node_attr_list.gain[i];
		}
	}
	
	return index;
}

Dt_node *
gen_decision_tree(Dt_node *dtn)
{
	int income_class;
	int splitting_criterion;
	int split_index;
	
	Adult_rec *ap;
	
	/* if tuples in D are all of the same class */
	if ((income_class = tuples_same_class(dtn)) != 0) {
		dtn->child_num = 0;
		dtn->classification = income_class;
		return dtn;
	}
	
	/* if attribute list is empty */
	if (get_attribute_num(&dtn->node_attr_list) == 0) {
		/* get the majority classification of the class */
		income_class = get_majority_class(dtn);
		dtn->classification = income_class;
		return dtn;
	}
	
	/* attribute_selection_method - information gain */
	split_index = cal_info_gain(dtn);
	
	dtn->attr_id = split_index;
	
	/* for each splitting_criterion */
	int i;
	for (i = 0; i < attr_range[split_index]; i ++) {
		/* create a node for this partition */
		Dt_node *np = (Dt_node *) malloc(sizeof(Dt_node));
		memset(np, '\0', sizeof(Dt_node));
		TAILQ_INIT(&np->adult_rec_tqh);

		/* move items into this partition */
		TAILQ_FOREACH(ap, &dtn->adult_rec_tqh, entries) {
			if (ap->workclass == attr_range[split_index])
				TAILQ_INSERT_TAIL(&np->adult_rec_tqh, ap, entries);
		}
		
		if (TAILQ_EMPTY(&np->adult_rec_tqh))
			/* find the majority of in this set */
			get_majority_class(np);
		else
			gen_decision_tree(np);
	}
	
	return dtn;
}
