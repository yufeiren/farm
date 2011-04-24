/* Insertion Sort in CLRS */

#include <stdio.h>
#include <stdlib.h>

void prtarray(int *array, int length)
{
	int i;
	for (i = 0; i < length; i++) {
		printf("\t%d", *(array + i));
		if ((i % 5 == 0) && (i != 0))
			printf("\n");
	}
	printf("\n");
}


void insertion(int *array, int length)
{
	int key;

	if ((length == 1) || (length == 0))
		return;

	int i, j;
	for (i = 1; i < length; i++) {
		key = *(array + i);
		
		j = i - 1;
		while ((key < *(array + j)) && (j >= 0)) {
			*(array + j + 1) = *(array + j);
			j --;
		}
		j ++;
		*(array + j) = key;
	}
}

int
main(int argc, char **argv)
{
	int number = atoi(argv[1]);

	int *ip = (int *) malloc(number * sizeof(int));

	int i;
	for (i = 0; i < number; i ++)
		*(ip + i) = rand() % 10000;
	
	printf("before sort\n");
/*	prtarray(ip, number);*/
	insertion(ip, number);
	printf("\n\nafter sort\n");
/*	prtarray(ip, number);*/
	
}

