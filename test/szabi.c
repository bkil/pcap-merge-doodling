#include "szabi.h"
#include "libtest.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static int
test_random_overflow (void)
{
  long min = LONG_MAX;
  long rand = get_random_above (min);
  for (size_t i = 0; i < 1000; i++)
    {
      if (rand != -1)
	{
	  printf
	    ("=test_random_overflow: should fail try=%ld, rand=%ld, min=%ld\n",
	     i, rand, min);
	  return 1;
	}
    }
  return 0;
}

static int
test_random_negative (void)
{
  long min = -1;
  long rand = get_random_above (min);
  for (size_t i = 0; i < 1000; i++)
    {
      if (!(rand > min))
	{
	  printf ("=test_random_negative: failed try=%ld, rand=%ld, min=%ld\n",
		  i, rand, min);
	  return 1;
	}
    }
  return 0;
}

static int
test_random (void)
{
  return test_random_negative () + test_random_overflow ();
}

typedef struct TestCase
{
  long potential_prime;
  bool expected_result;
} TestCase;


static int
test_prime (void)
{
  TestCase test_cases[] = {
    {-3, false},
    {-1, false},
    {0, false},
    {1, false},
    {2, true},
    {3, true},
    {4, false},
    {42, false},
    {43, true},
    {45, false},
    {101, true}
  };
  int fails = 0;
  for (size_t i = 0; i < array_len (test_cases); i++)
    {
      long potential_prime = test_cases[i].potential_prime;
      bool expected_result = test_cases[i].expected_result;
      bool result = is_prime (potential_prime);
      if (result != expected_result)
	{
	  printf ("=test_prime expected is_prime(%ld)=%d, got %d\n",
		  potential_prime, expected_result, result);
	  fails++;
	}
    }
  return fails > 0;
}

int
main (int argc, char *argv[])
{
  return test_random () + test_prime ();
}
