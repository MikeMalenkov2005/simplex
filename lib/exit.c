#include <signal.h>
#include <simplex.h>

#include "crt.h"

#define AT_EXIT_MAX 32

static void (*at_exit[AT_EXIT_MAX])(void);
static int at_exit_count = 0;
static int at_exit_lock = 0;

void abort(void)
{
  raise(SIGABRT);
  sys_group_exit(127);
}

int atexit(void (*func)(void))
{
  int result = -1;
  if (func)
  {
    __crt_lock(&at_exit_lock);
    if (at_exit_count < AT_EXIT_MAX)
    {
      at_exit[at_exit_count++] = func;
      result = 0;
    }
    __crt_unlock(&at_exit_lock);
  }
  return result;
}

void exit(int status)
{
  __crt_lock(&at_exit_lock);
  while (at_exit_count--) at_exit[at_exit_count]();
  sys_group_exit(status);
}

