#include <stdio.h>
#include "multiway.h"

int
main(int argc, char **argv)
{
  /* parse define.txt */
  parse_define();

  /* parse raw data to trunk data */
  raw2chunk(F_RAW, F_CHUNK);

  /* load chunk one by one */
  int i;
  totaltrunknum = 1;
  for (i = 0; i < dimnum; i ++) {
    if (dimlen[i] % chklen[i] == 0)
      totaltrunknum *= dimlen[i] / chklen[i];
    else
      totaltrunknum *= (dimlen[i] / chklen[i] + 1);
  }

  for (i = 0; i < totaltrunknum; i ++) {
    load_chunk(i);
  }

  exit(EXIT_SUCCESS);
}
