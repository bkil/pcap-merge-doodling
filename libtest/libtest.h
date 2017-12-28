#ifndef TEST_TESTLIB_H_
#define TEST_TESTLIB_H_

#include <sys/types.h>

off_t get_file_size (const char *filename);
int expect_string_in_file (const char *filename, const char *content);
int expect_bytes_in_file (const char *filename, const char *content,
			  size_t len);

typedef int (*main_function_t) (int argc, char *argv[]);
int run_main_function (main_function_t fun, const char *const argv[]);

#endif
