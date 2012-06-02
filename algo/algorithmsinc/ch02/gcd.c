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
		u = u - v;
	}
	return v;
}

int
main()
{
	int u, v;
	scanf("%d %d", &u, &v);
	gcd(u, v);
	printf("%d %d %d\n", u, v, gcd(u, v));

	exit(0);
}
