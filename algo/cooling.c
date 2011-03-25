/* Just for fun
 * question is here,
 * http://www.quora.com/about/challenges
 */

#define ROWNED 0
#define RNOTOWNED 1
#define RSTART 2
#define REND 3

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

static Room * check_next(Room *);

static int m;
static int n;

int
main(int argc, char **argv)
{

	return 0;
}

static Room *
check_next(Room *r)
{
	Room *next;
	if ((next = get_upper(r) != NULL) {
		
	}

	if (r->isin == 1)
		return NULL;
	
	if (r->value == REND) {
		check_path();
		return NULL;
	}
	
	
	
}

static Room *
get_upper(Room *r)
{
	if (r == NULL)
		return NULL;
	
	if ((r->y == 0) || (r->isin == 1) || (r->value != 
		return NULL;
	
	return (static Room *) addr[m * (r->y - 1) + r->x];
}


static Room *
get_lower(Room *r)
{
	if (r == NULL)
		return NULL;

	if (r->y == n - 1)
		return NULL;

	return (static Room *) addr[m * (r->y + 1) + r->x];
}


static Room * get_left(Room *r);
{
	if (r == NULL)
		return NULL;

	if (r->x == 0)
		return NULL;

	return (static Room *) addr[m * r->y + r->x - 1];
}


static Room * get_right(Room *);
{
	if (r == NULL)
		return NULL;

	if (r->y == m - 1)
		return NULL;

	return (static Room *) addr[m * r->y + r->x + 1];
}

