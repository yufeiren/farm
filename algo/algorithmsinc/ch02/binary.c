#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
binary(int x)
{
	char s[128];
	int i;
	memset(s, '\0', 128);
	i = 0;

	while (x > 0) {
		if (x & 1)
			*(s + i) = '1';
		else
			*(s + i) = '0';
		x = x >> 1;
		i ++;
	}

	while (i-- > 0) {
		printf("%c", *(s + i));
	}
	printf("\n");
}

int
main()
{
	int x;
	scanf("%d", &x);
	binary(x);

	exit(0);
}
