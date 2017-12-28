#include "1-cat.h"
#include "libtest.h"

#include <stddef.h>
#include <stdio.h>

const char *executable = "main/1-cat.elf";
const char *writable = "var/out~";

static int
test_no_parameters (void)
{
  const char *const cmd[] = { executable, NULL };
  int result = run_main_function (main_1_cat, cmd);
  if (result != 1)
    {
      fprintf (stderr, "= test_no_parameters: should have failed\n");
      return 1;
    }
  return 0;
}

static int
test_too_many_parameters (void)
{
  const char *const cmd[] =
    { executable, "test/data/single-char.txt", writable, "3", NULL };
  int result = run_main_function (main_1_cat, cmd);
  if (result != 1)
    {
      fprintf (stderr, "= test_too_many_parameters: should have failed\n");
      return 1;
    }
  return 0;
}

static int
test_missing_input (void)
{
  const char *const cmd[] = { executable, "missing-input", writable, NULL };
  int result = run_main_function (main_1_cat, cmd);
  if (result != 1)
    {
      fprintf (stderr, "= test_missing_input: should have failed\n");
      return 1;
    }
  return 0;
}

static int
test_unreadable_input (void)
{
  const char *const cmd[] = { executable, ".", writable, NULL };
  int result = run_main_function (main_1_cat, cmd);
  if (result != 1)
    {
      fprintf (stderr, "= test_unreadable_input: should have failed\n");
      return 1;
    }
  return 0;
}

static int
test_create_output_fail (void)
{
  const char *const cmd[] =
    { executable, "test/data/single-char.txt", ".", NULL };
  int result = run_main_function (main_1_cat, cmd);
  if (result != 1)
    {
      fprintf (stderr, "= test_create_output_fail: should have failed\n");
      return 1;
    }
  return 0;
}

static int
test_empty_input (void)
{
  const char *const cmd[] =
    { executable, "test/data/empty.txt", writable, NULL };
  int result = run_main_function (main_1_cat, cmd);
  if (result != 0)
    {
      fprintf (stderr, "= test_empty_input: should have succeeded\n");
      return 1;
    }
  if (get_file_size (writable) != 0)
    {
      fprintf (stderr, "= test_single_character: expected empty file\n");
      return 1;
    }
  return 0;
}

static int
test_single_character (void)
{
  const char *const cmd[] =
    { executable, "test/data/single-char.txt", writable, NULL };
  int result = run_main_function (main_1_cat, cmd);
  if (result != 0)
    {
      fprintf (stderr, "= test_single_character: should have succeeded\n");
      return 1;
    }
  if (expect_string_in_file (writable, "a") != 0)
    {
      fprintf (stderr, "= test_single_character: expected single character\n");
      return 1;
    }
  return 0;
}

static int
test_many_characters (void)
{
  const char *const cmd[] =
    { executable, "test/data/many-char.txt", writable, NULL };
  int result = run_main_function (main_1_cat, cmd);
  if (result != 0)
    {
      fprintf (stderr, "= test_many_characters: should have succeeded\n");
      return 1;
    }
  if (expect_string_in_file (writable, "many characters") != 0)
    {
      fprintf (stderr, "= test_many_characters: mismatch\n");
      return 1;
    }
  return 0;
}

int
main (int argc, char *argv[])
{
  return test_no_parameters () + test_too_many_parameters () +
    test_missing_input () + test_unreadable_input () +
    test_create_output_fail () + test_empty_input () +
    test_single_character () + test_many_characters ();
}
