/*
 * my first solution showed the number solution is 301716.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/queue.h>

#define DIR_UP		0
#define DIR_RIGHT	1
#define DIR_DOWN	2
#define DIR_LEFT	3

#define MAZE_WALL	'x'
#define MAZE_PATH	'o'

#define MAZE_DEADEND	9
#define MAZE_NORMAL	8


/* algorithm:
 * if (next host is connected)
 *     get the next host;
 * if (next host is already in the list)
 *     could not add this host;
 * if (next host is REND)
 *     check the list;
 * add the next host
 */

struct maze_point {
	int value;	/* o, x, A B C D E */
	int status;	/* if not 'x', deadend, normal */
	int x;
	int y;

	TAILQ_ENTRY(maze_point) entries;
};
typedef struct maze_point Maze_point;

TAILQ_HEAD(, maze_point) 	maze_point_tqh;

static Maze_point * get_upper(Maze_point *);
static Maze_point * get_lower(Maze_point *);
static Maze_point * get_left(Maze_point *);
static Maze_point * get_right(Maze_point *);


static int check_next(Maze_point *, int);

static int apple_num;
static Maze_point mp[36][36];

void print_path();

int
main(int argc, char **argv)
{
	int i, j;
	char line[1024];

	Maze_point mp[36][36];

	TAILQ_INIT(&maze_point_tqh);
	apple_num = 0;
	
	FILE *input = fopen(argv[1], "r");
	if (input == NULL) {
		fprintf(stderr, "can not open file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	for (i = 0; i < 36; i++) {
		memset(line, '\0', 1024);
		if (fgets(line, 1024, input) == NULL)
			fprintf(stderr, "wrong file format\n");
		
		for (j = 0; j < 36; j ++) {
			mp[i][j].value = *(line + j);
			mp[i][j].x = i;
			mp[i][j].y = j;
		}
		
		printf("line[%d]: %s\n", i, line);
	}

	check_next(&mp[35][0], DIR_LEFT);
	
	exit(EXIT_SUCCESS);
}

static int
check_next(Maze_point *p, int previous)
{
	Maze_point *pup;
	Maze_point *pright;
	Maze_point *pdown;
	Maze_point *pleft;
	
	pup = pright = pdown = pleft = NULL;
	
	/* if get all the apples */
	if (apple_num == 5) {
		printf("Completed!!!\n");
		print_path();
		exit(EXIT_SUCCESS);
	}
	
	/* insert p into list */
	TAILQ_INSERT_TAIL(&maze_point_tqh, p, entries);
	
	switch (previous) {
	case DIR_DOWN:
		pup = get_upper(p);
		pright = get_right(p);
		pleft = get_left(p);
		break;
	case DIR_LEFT:
		pup = get_upper(p);
		pright = get_right(p);
		pdown = get_lower(p);
		break;
	case DIR_UP:
		pright = get_right(p);
		pdown = get_lower(p);
		pleft = get_left(p);
		break;
	case DIR_RIGHT:
		pup = get_upper(p);
		pdown = get_lower(p);
		pleft = get_left(p);
		break;
	default:
		fprintf(stderr, "... something wrong\n");
		exit(EXIT_FAILURE);
		break;
	}
	
	/* assume this point is deadend, except it has a valuable next step */
	p->status = MAZE_DEADEND;
	
	if (pup != NULL)
		if (check_next(pup, DIR_DOWN) != MAZE_DEADEND)
			p->status = MAZE_NORMAL;
	if (pright != NULL)
		if (check_next(pright, DIR_LEFT) != MAZE_DEADEND)
			p->status = MAZE_NORMAL;
	if (pdown != NULL)
		if (check_next(pright, DIR_UP) != MAZE_DEADEND)
			p->status = MAZE_NORMAL;
	if (pleft != NULL)
		if (check_next(pleft, DIR_RIGHT) != MAZE_DEADEND)
			p->status = MAZE_NORMAL;
	
	if (p->status == MAZE_DEADEND) {
		/* p is an apple */
		if (p->value != MAZE_PATH)
			print_path();
		
		/* remove p from list */
		TAILQ_REMOVE(&maze_point_tqh, p, entries);
	}
	
	return p->status;
}

void
print_path()
{
	Maze_point *p;
	
	TAILQ_FOREACH(p, &maze_point_tqh, entries)
		printf("(%d, %d),\n", p->x, p->y);
	
	return;
}


static Maze_point *
get_upper(Maze_point *p)
{
	Maze_point *tmp;
	
	if (p->x == 0)
		return NULL;
	
	tmp = &mp[p->x - 1][p->y];
	
	if (tmp->value == MAZE_WALL)
		return NULL;
	
	if (tmp->status == MAZE_DEADEND)
		return NULL;

	if (tmp->value != MAZE_PATH)
		apple_num ++;
	
	return tmp;
}

static Maze_point *
get_lower(Maze_point *p)
{
	Maze_point *tmp;
	
	if (p->x == 35)
		return NULL;
	
	tmp = &mp[p->x + 1][p->y];
	
	if (tmp->value == MAZE_WALL)
		return NULL;
	
	if (tmp->status == MAZE_DEADEND)
		return NULL;

	if (tmp->value != MAZE_PATH)
		apple_num ++;
	
	return tmp;
}

static Maze_point *
get_left(Maze_point *p)
{
	Maze_point *tmp;
	
	if (p->y == 0)
		return NULL;
	
	tmp = &mp[p->x][p->y - 1];
	
	if (tmp->value == MAZE_WALL)
		return NULL;
	
	if (tmp->status == MAZE_DEADEND)
		return NULL;

	if (tmp->value != MAZE_PATH)
		apple_num ++;
	
	return tmp;
}

static Maze_point *
get_right(Maze_point *p)
{
	Maze_point *tmp;
	
	if (p->y == 35)
		return NULL;
	
	tmp = &mp[p->x][p->y + 1];
	
	if (tmp->value == MAZE_WALL)
		return NULL;
	
	if (tmp->status == MAZE_DEADEND)
		return NULL;

	if (tmp->value != MAZE_PATH)
		apple_num ++;
	
	return tmp;
}
