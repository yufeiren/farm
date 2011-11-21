#ifndef __DEBUG_H
#define __DEBUG_H

#ifdef DEBUG
# define DPRINTF(arg)	printf arg
#else
# define DPRINTF(arg)
#endif
	
#endif