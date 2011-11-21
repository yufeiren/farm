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
		if (strlen(line) == 0)
			continue;
		DPRINTF(("parse line: %s\n", line));
		Adult_rec *rp = (Adult_rec *) malloc(sizeof(Adult_rec));
		memset(rp, '\0', sizeof(Adult_rec));
		line[strlen(line) - 1] = ',';
		
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

		start = line;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		rp->age = atoi(buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(workclass, buf);

		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		rp->fnlwgt = atoi(buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(education, buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		rp->education_num = atoi(buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(marital_status, buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(occupation, buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(relationship, buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(race, buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(sex, buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		rp->capital_gain = atoi(buf);

		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		rp->capital_loss = atoi(buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		rp->hours_per_week = atoi(buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(native_country, buf);
		
		start = end + 2;
		end = strchr(start, ',');
		memset(buf, '\0', sizeof(buf));
		memcpy(buf, start, end - start);
		strcpy(income_class, buf);
		
/*		sscanf(line, "%d, %s, %d, %s, %d, %s, %s, %s, %s, %s, %d, %d, %d, %s, %s\n",
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
		); */

/* workclass: Private, Self-emp-not-inc, Self-emp-inc, Federal-gov, Local-gov, State-gov, Without-pay, Never-worked. */
		if (strcmp(workclass, "Private") == 0)
			rp->workclass = 1;
		else if (strcmp(workclass, "Self-emp-not-inc") == 0)
			rp->workclass = 2;
		else if (strcmp(workclass, "Self-emp-inc") == 0)
			rp->workclass = 3;
		else if (strcmp(workclass, "Federal-gov") == 0)
			rp->workclass = 4;
		else if (strcmp(workclass, "Local-gov") == 0)
			rp->workclass = 5;
		else if (strcmp(workclass, "State-gov") == 0)
			rp->workclass = 6;
		else if (strcmp(workclass, "Without-pay") == 0)
			rp->workclass = 7;
		else if (strcmp(workclass, "Never-worked") == 0)
			rp->workclass = 8;
		else
			rp->workclass = 0;

/*fnlwgt: continuous.
education: Bachelors, Some-college, 11th, HS-grad, Prof-school, Assoc-acdm, Assoc-voc, 9th, 7th-8th, 12th, Masters, 1st-4th, 10th, Doctorate, 5th-6th, Preschool.*/
		if (strcmp(education, "Bachelors") == 0)
			rp->education = 1;
		else if (strcmp(education, "Some-college") == 0)
			rp->education = 2;
		else if (strcmp(education, "11th") == 0)
			rp->education = 3;
		else if (strcmp(education, "HS-grad") == 0)
			rp->education = 4;
		else if (strcmp(education, "Prof-school") == 0)
			rp->education = 5;
		else if (strcmp(education, "Assoc-acdm") == 0)
			rp->education = 6;
		else if (strcmp(education, "Assoc-voc") == 0)
			rp->education = 7;
		else if (strcmp(education, "9th") == 0)
			rp->education = 8;
		else if (strcmp(education, "7th-8th") == 0)
			rp->education = 9;
		else if (strcmp(education, "12th") == 0)
			rp->education = 10;
		else if (strcmp(education, "Masters") == 0)
			rp->education = 11;
		else if (strcmp(education, "1st-4th") == 0)
			rp->education = 12;
		else if (strcmp(education, "10th") == 0)
			rp->education = 13;
		else if (strcmp(education, "Doctorate") == 0)
			rp->education = 14;
		else if (strcmp(education, "5th-6th") == 0)
			rp->education = 15;
		else if (strcmp(education, "Preschool") == 0)
			rp->education = 16;
		else
			rp->education = 0;
		
/*education-num: continuous.
marital-status: Married-civ-spouse, Divorced, Never-married, Separated, Widowed, Married-spouse-absent, Married-AF-spouse.*/

		if (strcmp(marital_status, "Married-civ-spouse") == 0)
			rp->marital_status = 1;
		else if (strcmp(marital_status, "Divorced") == 0)
			rp->marital_status = 2;
		else if (strcmp(marital_status, "Never-married") == 0)
			rp->marital_status = 3;
		else if (strcmp(marital_status, "Separated") == 0)
			rp->marital_status = 4;
		else if (strcmp(marital_status, "Widowed") == 0)
			rp->marital_status = 5;
		else if (strcmp(marital_status, "Married-spouse-absent") == 0)
			rp->marital_status = 6;
		else if (strcmp(marital_status, "Married-AF-spouse") == 0)
			rp->marital_status = 7;
		else
			rp->marital_status = 0;

/*
occupation: Tech-support, Craft-repair, Other-service, Sales, Exec-managerial, Prof-specialty, Handlers-cleaners, Machine-op-inspct, Adm-clerical, Farming-fishing, Transport-moving, Priv-house-serv, Protective-serv, Armed-Forces. */

		if (strcmp(occupation, "Tech-support") == 0)
			rp->occupation = 1;
		else if (strcmp(occupation, "Craft-repair") == 0)
			rp->occupation = 2;
		else if (strcmp(occupation, "Other-service") == 0)
			rp->occupation = 3;
		else if (strcmp(occupation, "Sales") == 0)
			rp->occupation = 4;
		else if (strcmp(occupation, "Exec-managerial") == 0)
			rp->occupation = 5;
		else if (strcmp(occupation, "Prof-specialty") == 0)
			rp->occupation = 6;
		else if (strcmp(occupation, "Handlers-cleaners") == 0)
			rp->occupation = 7;
		else if (strcmp(occupation, "Machine-op-inspct") == 0)
			rp->occupation = 8;
		else if (strcmp(occupation, "Adm-clerical") == 0)
			rp->occupation = 9;
		else if (strcmp(occupation, "Farming-fishing") == 0)
			rp->occupation = 10;
		else if (strcmp(occupation, "Transport-moving") == 0)
			rp->occupation = 11;
		else if (strcmp(occupation, "Priv-house-serv") == 0)
			rp->occupation = 12;
		else if (strcmp(occupation, "Protective-serv") == 0)
			rp->occupation = 13;
		else if (strcmp(occupation, "Armed-Forces") == 0)
			rp->occupation = 14;
		else
			rp->occupation = 0;

/*relationship: Wife, Own-child, Husband, Not-in-family, Other-relative, Unmarried.*/
		if (strcmp(relationship, "Wife") == 0)
			rp->relationship = 1;
		else if (strcmp(relationship, "Own-child") == 0)
			rp->relationship = 2;
		else if (strcmp(relationship, "Husband") == 0)
			rp->relationship = 3;
		else if (strcmp(relationship, "Not-in-family") == 0)
			rp->relationship = 4;
		else if (strcmp(relationship, "Other-relative") == 0)
			rp->relationship = 5;
		else if (strcmp(relationship, "Unmarried") == 0)
			rp->relationship = 6;
		else
			rp->relationship = 0;
		
/*race: White, Asian-Pac-Islander, Amer-Indian-Eskimo, Other, Black.*/
		if (strcmp(race, "White") == 0)
			rp->race = 1;
		else if (strcmp(race, "Asian-Pac-Islander") == 0)
			rp->race = 2;
		else if (strcmp(race, "Amer-Indian-Eskimo") == 0)
			rp->race = 3;
		else if (strcmp(race, "Other") == 0)
			rp->race = 4;
		else if (strcmp(race, "Black") == 0)
			rp->race = 5;
		else
			rp->race = 0;
		
/*sex: Female, Male.*/
		if (strcmp(race, "Female") == 0)
			rp->sex = 1;
		else if (strcmp(race, "Male") == 0)
			rp->race = 2;
		else
			rp->race = 0;

/*capital-gain: continuous.
capital-loss: continuous.
hours-per-week: continuous.
native-country: United-States, Cambodia, England, Puerto-Rico, Canada, Germany, Outlying-US(Guam-USVI-etc), India, Japan, Greece, South, China, Cuba, Iran, Honduras, Philippines, Italy, Poland, Jamaica, Vietnam, Mexico, Portugal, Ireland, France, Dominican-Republic, Laos, Ecuador, Taiwan, Haiti, Columbia, Hungary, Guatemala, Nicaragua, Scotland, Thailand, Yugoslavia, El-Salvador, Trinadad&Tobago, Peru, Hong, Holand-Netherlands.*/
		
		if (strcmp(native_country, "United-States") == 0)
			rp->native_country = 1;
		else if (strcmp(native_country, "Cambodia") == 0)
			rp->native_country = 2;
		else if (strcmp(native_country, "England") == 0)
			rp->native_country = 3;
		else if (strcmp(native_country, "Puerto-Rico") == 0)
			rp->native_country = 4;
		else if (strcmp(native_country, "Canada") == 0)
			rp->native_country = 5;
		else if (strcmp(native_country, "Germany") == 0)
			rp->native_country = 6;
		else if (strcmp(native_country, "Outlying-US(Guam-USVI-etc)") == 0)
			rp->native_country = 7;
		else if (strcmp(native_country, "India") == 0)
			rp->native_country = 8;
		else if (strcmp(native_country, "Japan") == 0)
			rp->native_country = 9;
		else if (strcmp(native_country, "Greece") == 0)
			rp->native_country = 10;
		else if (strcmp(native_country, "South") == 0)
			rp->native_country = 11;
		else if (strcmp(native_country, "China") == 0)
			rp->native_country = 12;
		else if (strcmp(native_country, "Cuba") == 0)
			rp->native_country = 13;
		else if (strcmp(native_country, "Iran") == 0)
			rp->native_country = 14;
		else if (strcmp(native_country, "Honduras") == 0)
			rp->native_country = 15;
		else if (strcmp(native_country, "Philippines") == 0)
			rp->native_country = 16;
		else if (strcmp(native_country, "Italy") == 0)
			rp->native_country = 17;
		else if (strcmp(native_country, "Poland") == 0)
			rp->native_country = 18;
		else if (strcmp(native_country, "Jamaica") == 0)
			rp->native_country = 19;
		else if (strcmp(native_country, "Vietnam") == 0)
			rp->native_country = 20;
		else if (strcmp(native_country, "Mexico") == 0)
			rp->native_country = 21;
		else if (strcmp(native_country, "Portugal") == 0)
			rp->native_country = 22;
		else if (strcmp(native_country, "Ireland") == 0)
			rp->native_country = 23;
		else if (strcmp(native_country, "France") == 0)
			rp->native_country = 24;
		else if (strcmp(native_country, "Dominican-Republic") == 0)
			rp->native_country = 25;
		else if (strcmp(native_country, "Laos") == 0)
			rp->native_country = 26;
		else if (strcmp(native_country, "Ecuador") == 0)
			rp->native_country = 27;
		else if (strcmp(native_country, "Taiwan") == 0)
			rp->native_country = 28;
		else if (strcmp(native_country, "Haiti") == 0)
			rp->native_country = 29;
		else if (strcmp(native_country, "Columbia") == 0)
			rp->native_country = 30;
		else if (strcmp(native_country, "Hungary") == 0)
			rp->native_country = 31;
		else if (strcmp(native_country, "Guatemala") == 0)
			rp->native_country = 32;
		else if (strcmp(native_country, "Nicaragua") == 0)
			rp->native_country = 33;
		else if (strcmp(native_country, "Scotland") == 0)
			rp->native_country = 34;
		else if (strcmp(native_country, "Thailand") == 0)
			rp->native_country = 35;
		else if (strcmp(native_country, "Yugoslavia") == 0)
			rp->native_country = 36;
		else if (strcmp(native_country, "El-Salvador") == 0)
			rp->native_country = 37;
		else if (strcmp(native_country, "Trinadad&Tobago") == 0)
			rp->native_country = 38;
		else if (strcmp(native_country, "Peru") == 0)
			rp->native_country = 39;
		else if (strcmp(native_country, "Hong") == 0)
			rp->native_country = 40;
		else if (strcmp(native_country, "Holand-Netherlands") == 0)
			rp->native_country = 41;
		else
			rp->native_country = 0;

/*income: <=50K, >50K.*/
		if (strcmp(income_class, "<=50K") == 0)
			rp->income_class = 1;
		else if (strcmp(income_class, ">50K") == 0)
			rp->income_class = 2;
		else
			rp->income_class = 0;

		TAILQ_INSERT_TAIL(&adult_rec_tqh, rp, entries);

		prt_adult_int(rp, "load");
		
		memset(line, '\0', sizeof(line));
		
		total_trans ++;
	}

	fclose(fp);
	
	return total_trans;
}


void
prt_adult_int(Adult_rec *rp, char *msg)
{
	printf("%s: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
		msg, 
		rp->age,
		rp->workclass,
		rp->fnlwgt,
		rp->education,
		rp->education_num,
		rp->marital_status,
		rp->occupation,
		rp->relationship,
		rp->race,
		rp->sex,
		rp->capital_gain,
		rp->capital_loss,
		rp->hours_per_week,
		rp->native_country,
		rp->income_class
	);
	
	return;
}
