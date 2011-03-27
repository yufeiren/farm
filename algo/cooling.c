/* Just for fun
 * question is here,
 * http://www.quora.com/about/challenges
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/queue.h>

#define ROWNED 0
#define RNOTOWNED 1
#define RSTART 2
#define REND 3

int room_not_owned_num;
int room_owned_num;

/* suppose the data center is m * n,
 * the horizontal connection is (m - 1) * n,
 * and the vertical connection is m * (n - 1).
 */

/* algorithm:
 * if (next host is connected)
 *     get the next host;
 * if (next host is already in the list)
 *     could not add this host;
 * if (next host is REND)
 *     check the list;
 * add the next host
 */

struct room {
	int value;
	int x;
	int y;
	int isin;

	TAILQ_ENTRY(room) entries;
};
typedef struct room Room;

struct hconn {
	int x;
	int y;
};
typedef struct hconn Hconn;

struct vconn {
	int x;
	int y;
};
typedef struct vconn Vconn;

TAILQ_HEAD(, room) 	room_tqh;

long addr[1024];

static Room * get_upper(Room *);
static Room * get_lower(Room *);
static Room * get_left(Room *);
static Room * get_right(Room *);

static void check_next(Room *);
static void check_path();

static int m;
static int n;

static int inlist;

int
main(int argc, char **argv)
{
	int i, j;
	char line[1024];
	int value[1024];

	scanf("%d %d", &m, &n);
	memset(line, '\0', 1024);
	fgets(line, 1024, stdin);

	TAILQ_INIT(&room_tqh);

	Room *r;
	for (i = 0; i < n; i++) {
		memset(line, '\0', 1024);
		fgets(line, 1024, stdin);
		sscanf(line, "%d %d %d %d %d %d %d", \
			&value[0], &value[1], &value[2], &value[3], &value[4], &value[5], &value[6]);
		for (j = 0; j < m; j++) {
			r = (Room *) malloc(sizeof(Room));
			r->x = i;
			r->y = j;
			r->value = value[j];
			r->isin = 0;

			if (r->value == RNOTOWNED)
				room_not_owned_num ++;

			addr[i * m + j] = (long) r;
		}
	}

	room_owned_num = m * n - room_not_owned_num;
	printf("%d %d\n", room_owned_num, room_not_owned_num);

	for (i = 0; i < 9; i++) {
		r = (Room *) addr[i];
		printf("(%d, %d) %d\n", r->x, r->y, r->value);
	}


	r = (Room *) addr[0];
	TAILQ_INSERT_TAIL(&room_tqh, r, entries);
	r->isin = 1;
	inlist ++;

	check_next(r);

	return 0;
}

static void 
check_next(Room *r)
{
	Room *next;

	int i;

	/* if get the end of the path */
	if (r->value == REND) {
		check_path();
		return;
	}

	for (i = 0; i < 4; i++) {
		if (i == 0)
			next = get_upper(r);
		else if (i == 1)
			next = get_lower(r);
		else if (i == 2)
			next = get_left(r);
		else
			next = get_right(r);

		if ((next == NULL) || (next->isin == 1) || (next->value == RNOTOWNED))
			continue;

		/* insert next to list */
		TAILQ_INSERT_TAIL(&room_tqh, next, entries);
		next->isin = 1;
		inlist ++;

		check_next(next);

		/* remove next from list */
		TAILQ_REMOVE(&room_tqh, next, entries);
		next->isin = 0;
		inlist --;
	}

/*	if (next == NULL) {
		the app could stop now
		printf("finish\n");
		exit(0);
	} */

	return;
}

static Room *
get_upper(Room *r)
{
	if (r == NULL)
		return NULL;
	
	if (r->x == 0) 
		return NULL;
	
	return (Room *) addr[m * (r->x - 1) + r->y];
}


static Room *
get_lower(Room *r)
{
	if (r == NULL)
		return NULL;
	if (r->x == n - 1)
		return NULL;

	return (Room *) addr[m * (r->x + 1) + r->y];
}


static Room * get_left(Room *r)
{
	if (r == NULL)
		return NULL;

	if (r->y == 0)
		return NULL;

	return (Room *) addr[m * r->x + r->y - 1];
}


static Room * get_right(Room *r)
{
	if (r == NULL)
		return NULL;

	if (r->y == m - 1)
		return NULL;

	return (Room *) addr[m * r->x + r->y + 1];
}

static void
check_path()
{
	Room *r;

	if (inlist == room_owned_num) {
		TAILQ_FOREACH(r, &room_tqh, entries)
			printf("(%d, %d) -> ", r->x, r->y);
		printf("\n");
	}
		
	return;
}

