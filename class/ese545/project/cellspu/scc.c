/* cell spu compiler */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

static void rmsep(char *dst, const char *src)
{
  int c;
  int srcpos, dstpos;
  int issep; /* the previous is a seperator */
  srcpos = 0;
  dstpos = 0;
  issep = 0;

  while (((c = *(src + srcpos++)) != '\0') && (c != '\n')) {
    if (c == ' ' || c == '\t' || c == ',') {
      if (issep == 0) {
        *(dst + dstpos++) = ' ';
	issep = 1;
      }
    } else {
      *(dst + dstpos++) = c;
      issep = 0;
    }
  }

  *(dst + dstpos) = '\0';

  return;
}

/*static void
setbits(uint32 *ip, int start, int length, uint32 value)
{
  int sbyte = start / 8;

  switch (sbyte) {
  case 0:

    break;
  }

  return;
}

static uint32_t op2hex(char *op)
{
  uint32_t instr;

  instr = 0;
  if (strcmp(op[0], "a") == 0) {
    instr |= 0x18000000;
    instr |= atoi(op[3]) << 14;
    instr |= atoi(op[2]) << 7;
    instr |= atoi(op[1]);
  }

  return instr;
}*/


const char *byte_to_binary(int x)
{
    static char b[9];
    memset(b, '\0', 9);

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

static int cellspu2hex(char *obj, const char *src)
{
  char newsrc[1024];
  memset(newsrc, '\0', 1024);

  char op[5][16];
  int i;

  uint32_t instr;

  printf("=======================\n");
  printf("original instruction is: \t%s\n", src);

  /* remove all seperator to a Space */
  rmsep(newsrc, src);

  printf("new instruction is: \t%s\n", newsrc);

  memset(&instr, '\0', 4);

  for (i = 0; i < 5; i ++) {
    memset(op[i], '\0', 16);
  }

  char *s, *e;
  s = newsrc;

  for (i = 0; (e = strchr(s, ' ')) != NULL; i ++) {
    memcpy(op[i], s, e - s);
    s = e + 1;
    printf("op[%d] = %s\n", i, op[i]);
  }
  strcpy(op[i], s);
  printf("op[%d] = %s\n", i, op[i]);

  /*  instr = op2hex(op);*/
	instr = 0;
  if (strcmp(op[0], "a") == 0) { /* Add Word */
    instr |= 0x18000000;
    instr |= atoi(op[3]) << 14;
    instr |= atoi(op[2]) << 7;
    instr |= atoi(op[1]);
  } else if (strcmp(op[0], "il") == 0) { /* immediate load word */
    instr |= 0x40800000;
    instr |= atoi(op[2]) << 7;
    instr |= atoi(op[1]);
  } else {
    printf("unrecgonized opcode: %s\n", op[0]);
    return -1;
  }

  sprintf(obj, "%08x", instr);

  char bcode[4][9];

  memcpy(bcode[0], byte_to_binary(((instr&0xff000000)>>24)&0x00ff), 8);
  memcpy(bcode[1], byte_to_binary(((instr&0x00ff0000)>>16)&0x00ff), 8);
  memcpy(bcode[2], byte_to_binary(((instr&0x0000ff00)>>8)&0x00ff), 8);
  memcpy(bcode[3], byte_to_binary((uint32_t)(instr&0x000000ff)), 8);

  printf("%s ", bcode[0]);
  printf("%s ", bcode[1]);
  printf("%s ", bcode[2]);
  printf("%s\n", bcode[3]);

  return 0;
}

int
main(int argc, char **argv)
{

  if (argc != 3) {
    printf("Usage: scc [source file] [binary object]\n");
    exit(EXIT_FAILURE);
  }

  char src[1024], obj[1024];
  FILE *srcfp, *objfp;
  char srcbuf[1024], objbuf[1024];
  int ret;

  memset(src, '\0', 1024);
  memset(obj, '\0', 1024);
  strcpy(src, argv[1]);
  strcpy(obj, argv[2]);

  srcfp = fopen(src, "r");
  if (srcfp == NULL) {
    perror("fopen src file");
    exit(EXIT_FAILURE);
  }

  objfp = fopen(obj, "w");
  if (objfp == NULL) {
    perror("fopen obj file");
    exit(EXIT_FAILURE);
  }

  memset(srcbuf, '\0', 1024);
  memset(objbuf, '\0', 1024);
  ret = 0;
  while (fgets(srcbuf, 1024, srcfp) != NULL) {
    if ( (strlen(srcbuf) == 0) || (srcbuf[0] == '#') || (srcbuf[0] == ';') )
      continue;

    if ((ret = cellspu2hex(objbuf, srcbuf)) != 0) {
      printf("can not parse this line: \n\t%s\n", srcbuf);
      break;
    }

    fprintf(objfp, "%s\n", objbuf);
    memset(srcbuf, '\0', 1024);
    memset(objbuf, '\0', 1024);
  }

  if (ret == 0)
    printf("object file %s is created\n", obj);

  return 0;
}

