#include <stdio.h>

int G;

void
printg()
{
	printf("G = %d\n", G);

	return;
}

int
main()
{
	int i = 0;

	printf("i = %d\n", i);

	printf("hello\n");

	G = 5;
	printg();

	return 0;
}