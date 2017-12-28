#include "libtest.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

off_t
get_file_size (const char *filename)
{
  struct stat buf;
  if (stat (filename, &buf) < 0)
    {
      perror ("fstat() failed to get file size");
      return -1;
    }

  return buf.st_size;
}

static ssize_t
read_file_content (const char *filename, char *buf, size_t len)
{
  int fd = open (filename, O_RDONLY);
  ssize_t file_size = read (fd, buf, len);
  if (file_size < 0)
    {
      perror ("read() expect_characters_in_file error");
    }
  close (fd);
  return file_size;
}

int
expect_bytes_in_file (const char *filename, const char *content, size_t len)
{
  char buf[len + 1];
  off_t file_size = get_file_size (filename);
  if (file_size < 0)
    {
      return 1;
    }
  ssize_t got_size = read_file_content (filename, buf, len);
  if (got_size < 0)
    {
      return 1;
    }
  buf[got_size] = '\0';
  off_t slen = (off_t) len;
  if ((file_size != slen) || (got_size != slen))
    {
      fprintf (stderr, "=expected %lu characters in %s (%s), have %lu (%s)\n",
	       slen, filename, content, file_size, buf);
      return 1;
    }

  if (memcmp (content, buf, len) != 0)
    {
      fprintf (stderr, "=expected in %s characters (%s), have (%s)\n",
	       filename, content, buf);
      return 1;
    }
  return 0;
}

int
expect_string_in_file (const char *filename, const char *content)
{
  size_t len = strlen (content);
  return expect_bytes_in_file (filename, content, len);
}

static int
run_main_fun (main_function_t fun, int argc, const char *const argv[],
	      int argc_rw, char *argv_rw[])
{
  if (argc > 0)
    {
      size_t len = strlen (argv[0]);
      char buf[len + 1];
      memcpy (&buf, argv[0], len + 1);
      argv_rw[argc_rw] = (char *) &buf;
      return run_main_fun (fun, argc - 1, argv + 1, argc_rw + 1, argv_rw);
    }
  argv_rw[argc_rw] = NULL;
  return (*fun) (argc_rw, argv_rw);
}

int
run_main_function (main_function_t fun, const char *const argv[])
{
  int argc = 0;
  while (argv[argc] != NULL)
    {
      argc++;
    }
  char *argv_rw[argc + 1];
  return run_main_fun (fun, argc, argv, 0, argv_rw);
}
