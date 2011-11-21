#include <sys/queue.h>
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"

#ifndef __DECISION_TREE_H
#define __DECISION_TREE_H

#define DT_MAX_CLASS	8
#define DT_MAX_VALUE	8

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

struct dt_node {
	int attr_id;
	int is_class;		/* whether this node is a leaf node */
	int class_value;
	struct dt_node *child[DT_MAX_VALUE];
};
typedef struct dt_node Dt_node;

void gen_decision_tree();

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

TAILQ_HEAD(, adult_rec)		adult_rec_tqh;

/* return number of items in the dataset
 * -1:  error
 * >=0: number of items 
 */
int loaddata_adult_txt(char *filepath);

void prt_adult_int(Adult_rec *rp, char *msg);

#endif
