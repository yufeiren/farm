#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "dt.h"

int
loaddata_adult_txt(char *filepath)
{
	if (filepath == NULL)
		return -1;
	
	FILE *fp;
	char line[10240];
	char *start;
	char *end;
	char buf[64];
	int total_trans;
	
	char workclass[128];
	char education[128];
	char marital_status[128];
	char occupation[128];
	char relationship[128];
	char race[128];
	char sex[128];
	char capital_gain[128];
	char capital_loss[128];
	char hours_per_week[128];
	char native_country[128];
	char income_class[128];
	
	fp = fopen(filepath, "r");
	if (fp == NULL) {
		fprintf(stderr, "can not open file: %s\n", filepath);
		return -1;
	}

	/* TransactionID Item1 Item2 ... ItemN */
	memset(line, '\0', sizeof(line));
	total_trans = 0;
	while (fgets(line, 10240, fp) != NULL) {
DPRINTF(("parse line: %s\n", line));
		Adult_rec *rp = (Adult_rec *) malloc(sizeof(Adult_rec));
		memset(rp, '\0', sizeof(Adult_rec));

/* sample
53, Private, 144361, HS_grad, 9, Married_civ_spouse, Machine_op_inspct, Husband,
 White, Male, 0, 0, 38, United_States, <=50K
 */

		memset(workclass, '\0', 128);
		memset(education, '\0', 128);
		memset(marital_status, '\0', 128);
		memset(occupation, '\0', 128);
		memset(relationship, '\0', 128);
		memset(race, '\0', 128);
		memset(sex, '\0', 128);
		memset(capital_gain, '\0', 128);
		memset(capital_loss, '\0', 128);
		memset(hours_per_week, '\0', 128);
		memset(native_country, '\0', 128);
		memset(income_class, '\0', 128);

		sscanf(line, "%d, %s, %d, %s, %d, %s, %s, %s, %s, %s, %d, %d, %d, %s, %s\n",
			&rp->age,
			workclass,
			&rp->fnlwgt,
			education,
			&rp->education_num,
			marital_status,
			occupation,
			relationship,
			race,
			sex,
			&rp->capital_gain,
			&rp->capital_loss,
			&rp->hours_per_week,
			native_country,
			income_class
		);
		
		TAILQ_INSERT_TAIL(&adult_rec_tqh, rp, entries);

		memset(line, '\0', sizeof(line));
	}

	fclose(fp);
	
	return total_trans;
}