/* merge sort in CLRS */

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


void
merge(int *array, int start, int mid, int end)
{
	int llen = mid - start + 1;
	int rlen = end - mid;
	int *lp = (int *) malloc((llen + 1) * sizeof(int));
	int *rp = (int *) malloc((rlen + 1) * sizeof(int));

	int i, j, k;

	for (i = 0; i < llen; i++)
		*(lp + i) = *(array + start + i);

	for (i = 0; i < rlen; i++)
		*(rp + i) = *(array + mid + 1 + i);

	for (k = 0, i = 0, j = 0; \
		(k < end - start + 1) && (i < llen) && (j < rlen); \
		k ++) {

		if (*(lp + i) < *(rp + j)) {
			*(array + k + start) = *(lp + i);
			i ++;
		} else {
			*(array + k + start) = *(rp + j);
			j ++;
		}
	}

	if (i == llen)
		for ( ; k < end - start + 1; k ++)
			*(array + k + start) = *(rp + j++);
	else
		for ( ; k < end - start + 1; k ++)
			*(array + k + start) = *(lp + i++);

}

void
merge_sort(int *array, int start, int end)
{
	int mid;

	if (start < end) {
		mid = (start + end) / 2;
		merge_sort(array, start, mid);
		merge_sort(array, mid + 1, end);
		merge(array, start, mid, end);
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
/*        prtarray(ip, number);*/
        merge_sort(ip, 0, number - 1);
        printf("\n\nafter sort\n");
/*        prtarray(ip, number);*/
}

