#include <stdio.h>
#include <stdlib.h>

static int
gcd(int u, int v)
{
	int t;
	while (u > 0) {
		if (u < v) {
			t = u;
			u = v;
			v = t;
		}
		u = u % v;
	}
	return v;
}

int
main()
{
	int u, v;
	int uvgcd;
	printf("numerator:\n");
	scanf("%d", &u);
	printf("denominator:\n");
	scanf("%d", &v);
	uvgcd = gcd(u, v);
	printf("new numerator: %d\n", u / uvgcd);
	printf("new denominator: %d\n", v / uvgcd);

	exit(0);
}
