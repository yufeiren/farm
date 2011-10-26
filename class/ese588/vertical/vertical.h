#include <sys/queue.h>

struct Mw_item {
	int chunkid;
	int offset;
	int dim[MAX_MULTIWAY_DIM];
	void *content;

	TAILQ_ENTRY(Mw_item) entries;
};
typedef struct Mw_item MW_ITEM;



struct tidset {

};


struct patternset {
	
	
	struct tidset tids;
};
