#include "1-cat.h"

#include <stdio.h>
#include <errno.h>

static int
copy_given_descriptor (FILE * in, FILE * out)
{
  errno = 0;
  int ch = getc (in);
  while (ch != EOF)
    {
      int result = putc (ch, out);
      if (result == EOF)
	{
	  perror ("putc() write error");
	  return 1;
	}
      ch = getc (in);
    }
  if (errno != 0)
    {
      perror ("getc() read error");
      return 1;
    }
  return 0;
}

static int
copy_given_filename (const char *infile, const char *outfile)
{
  FILE *in = fopen (infile, "rb");
  if (!in)
    {
      perror ("fopen() failed to open input");
      return 1;
    }
  FILE *out = fopen (outfile, "wb");
  if (!out)
    {
      fclose (in);
      perror ("fopen() error creating output");
      return 1;
    }
  int result = copy_given_descriptor (in, out);
  fclose (in);
  fclose (out);
  return result;
}

int
main_1_cat (int argc, char *argv[])
{
  if (argc != 3)
    {
      fprintf (stderr, "usage: %s [infile] [outfile]\n", argv[0]);
      return 1;
    }
  return copy_given_filename (argv[1], argv[2]);
}
