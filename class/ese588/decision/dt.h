#include <sys/queue.h>
#include <stdlib.h>
#include <stdio.h>

#include <math.h>	/* for log */


#include "debug.h"

#ifndef __DECISION_TREE_H
#define __DECISION_TREE_H

#define DT_MAX_CLASS	8
#define DT_MAX_VALUE	8

#define DT_MAX_ATTRIBUTE	20
#define DT_MAX_ATTRANGE		100

#define ADULT_ATTRIBUTE_NUM	8

/* for each training data, update the following data structure */

struct class_set {
	int class_value;	/* the value of the class */
	int setsize;		/* number of id in the set */
	int *set;		/* the set for transaction id */
};
typedef struct class_set Class_set;

struct value_node {
	int attr_value;
	char desc[64];		/* description for this value */
	Class_set *cs[DT_MAX_CLASS];
	double info;
};
typedef struct value_node Value_node;

struct attr_node {
	int id;
	char desc[64];		/* description for this attribute */
	Value_node *vn[DT_MAX_VALUE];
	double info;
	
	TAILQ_ENTRY(attr_node) entries;
};
typedef struct attr_node Attr_node;


/* decision tree data structure */

/* IO Method */

/* raw data format
age: continuous.
workclass: Private, Self-emp-not-inc, Self-emp-inc, Federal-gov, Local-gov, State-gov, Without-pay, Never-worked.
fnlwgt: continuous.
education: Bachelors, Some-college, 11th, HS-grad, Prof-school, Assoc-acdm, Assoc-voc, 9th, 7th-8th, 12th, Masters, 1st-4th, 10th, Doctorate, 5th-6th, Preschool.
education-num: continuous.
marital-status: Married-civ-spouse, Divorced, Never-married, Separated, Widowed, Married-spouse-absent, Married-AF-spouse.
occupation: Tech-support, Craft-repair, Other-service, Sales, Exec-managerial, Prof-specialty, Handlers-cleaners, Machine-op-inspct, Adm-clerical, Farming-fishing, Transport-moving, Priv-house-serv, Protective-serv, Armed-Forces.
relationship: Wife, Own-child, Husband, Not-in-family, Other-relative, Unmarried.
race: White, Asian-Pac-Islander, Amer-Indian-Eskimo, Other, Black.
sex: Female, Male.
capital-gain: continuous.
capital-loss: continuous.
hours-per-week: continuous.
native-country: United-States, Cambodia, England, Puerto-Rico, Canada, Germany, Outlying-US(Guam-USVI-etc), India, Japan, Greece, South, China, Cuba, Iran, Honduras, Philippines, Italy, Poland, Jamaica, Vietnam, Mexico, Portugal, Ireland, France, Dominican-Republic, Laos, Ecuador, Taiwan, Haiti, Columbia, Hungary, Guatemala, Nicaragua, Scotland, Thailand, Yugoslavia, El-Salvador, Trinadad&Tobago, Peru, Hong, Holand-Netherlands. */

struct adult_rec {
	int age;
	int workclass;
	int fnlwgt;
	int education;
	int education_num;
	int marital_status;
	int occupation;
	int relationship;
	int race;
	int sex;
	int capital_gain;
	int capital_loss;
	int hours_per_week;
	int native_country;
	int income_class;	/* <=50K - 1, >50K - 2 */
	
	TAILQ_ENTRY(adult_rec) entries;
};
typedef struct adult_rec Adult_rec;


struct attr_list {
	int attr_id[DT_MAX_ATTRIBUTE];
	int attr_is_valid[DT_MAX_ATTRIBUTE];
	int poor_num[DT_MAX_ATTRIBUTE];
	int rich_num[DT_MAX_ATTRIBUTE];
	int poor_num_dist[DT_MAX_ATTRIBUTE][DT_MAX_ATTRANGE];
	int rich_num_dist[DT_MAX_ATTRIBUTE][DT_MAX_ATTRANGE];
	double info[DT_MAX_ATTRIBUTE];
	double gain[DT_MAX_ATTRIBUTE];
};
typedef struct attr_list Attr_list;


struct dt_node {
	Attr_list node_attr_list;
	int attr_id;		/* attr_id for this node */
	int child_val[DT_MAX_ATTRANGE];
	struct dt_node *children[DT_MAX_ATTRANGE];
	int child_num;			/* number of children */
	int classification;
	TAILQ_HEAD(, adult_rec)		adult_rec_tqh;
};
typedef struct dt_node Dt_node;

/* 			workclass,
			education,
			marital_status,
			occupation,
			relationship,
			race,
			sex,
			native_country,
			income_class */

int attr_range[9];

/* generate decision tree */
Dt_node *gen_decision_tree(Dt_node *dtn);

TAILQ_HEAD(, adult_rec)		adult_rec_tqh;

/* check if all the accords belong to the same class */
int tuples_same_class(Dt_node *dtn);

int get_attribute_num(Attr_list *alist);

int get_majority_class(Dt_node *dtn);

/* return the attribute of splitting */
int cal_info_gain(Dt_node *dtn);

/* return number of items in the dataset
 * -1:  error
 * >=0: number of items 
 */
int loaddata_adult_txt(char *filepath);

void prt_adult_int(Adult_rec *rp, char *msg);

#endif
