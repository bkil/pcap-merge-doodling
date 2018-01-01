#include "szabi.h"
#include "lib.h"

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>

typedef struct szabi_array
{
  size_t len;
  char *data;
} szabi_array;

static szabi_array *
new_szabi_array (size_t len)
{
  szabi_array *array = NULL;
  if (len < SIZE_MAX / sizeof (szabi_array))
    {
      array = malloc (len * sizeof (szabi_array));
      array->data = malloc (len * sizeof (array->data[0]));
      if (array->data != NULL)
	{
	  array->len = len;
	}
      else
	{
	  array->len = 0;
	}
    }
  return array;
}

static void
free_szabi_array (szabi_array * array)
{
  if (array != NULL)
    {
      free (array->data);
      array->data = NULL;
      free (array);
    }
}

long
get_random_above (long min)
{
  if (min > LONG_MAX - RAND_MAX)
    {
      return -1;
    }
  return min + random ();
}

bool
is_prime (long i)
{
  if (i < 2)
    {
      return false;
    }
  if (i == 2)
    {
      return true;
    }
  for (long j = 2; j < i; j++)
    {
      if (i % j == 0)
	{
	  return false;
	}
    }
  return true;
}

static void
szabi (void)
{
  FILE *files[] = { fopen ("1-cat.c", "rb"), fopen ("1-cat.c", "rb") };
  for (size_t i = 0; i < array_len (files); i++)
    {
      printf ("before fclose %lu\n", i);
      if (files[i])
	{
	  fclose (files[i]);
	}
    }

  szabi_array *array = NULL;

  const long x = get_random_above (2);
  if (is_prime (x))
    {

      array = new_szabi_array (2);
      array->data[0] = 'x';
      array->data[1] = 'y';
      for (size_t i = 0; i < array->len; i++)
	{
	  printf ("szabi[%lu] = %c\n", i, array->data[i]);
	}
    }

  free_szabi_array (array);
  printf ("all fclose'd\n");
}

int
main_szabi (int argc, char *argv[])
{
  printf ("loading szabi...\n");
  szabi ();
  return 0;
}
