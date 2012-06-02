#include <stdio.h>

#ifdef STDC_HEADERS
#include <stdlib.h>
#endif

#include <sys/types.h>

#ifdef HAVE_UTIME_H
#include <utime.h>
#endif

#ifndef HAVE_UTIME_NULL

#include <time.h>

#ifndef HAVE_STRUCT_UTIMBUF

struct utimbuf
{
  long actime;
  long modtime;
};

#endif

static int
utime_now (file)
     char *file;
{
  struct utimbuf now;

  now.actime = now.modtime = time (NULL);
  return utime (file, &now);
}

#define utime(f, p) utime_now (f)

#endif /* HAVE_UTIME_NULL  */

int
main (argc, argv)
     int argc;
     char **argv;
{
  if (argc != 2)
    {
      fprintf (stderr, "Usage: poke file\n");
      exit (1);
    }

  if (utime (argv[1], NULL) < 0)
    {
      perror ("utime");
      exit (1);
    }

  exit (0);
}
