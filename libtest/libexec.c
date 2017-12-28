#include "libexec.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static int
wait_for_child_result (pid_t pid)
{
  int status = 0;
  int result = waitpid (pid, &status, 0);
  if (result < 0)
    {
      perror ("waitpid() failed to wait for child");
      return 2;
    }
  if (WIFEXITED (status))
    {
      return WEXITSTATUS (status);
    }
  fprintf (stderr, "error: child died with status word=0x%x\n", status);
  return 2;
}

int
run (char *const argv[])
{
  pid_t pid = fork ();
  if (pid == 0)
    {
      exit (execv (argv[0], argv));
    }
  if (pid < 0)
    {
      perror ("fork() failed before run");
      return 2;
    }
  return wait_for_child_result (pid);
}
