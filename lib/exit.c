#include <signal.h>
#include <simplex.h>

#define AT_EXIT_MAX 32

static void (*at_exit[AT_EXIT_MAX])(void);
static int at_exit_count = 0;
static int at_exit_lock = -1;

void abort(void)
{
  raise(SIGABRT);
  sys_group_exit(127);
}

int atexit(void (*func)(void))
{
  int result = 0;
  if (func)
  {
    while (sys_acquire(&at_exit_lock)) sys_switch();
    if (at_exit_count >= AT_EXIT_MAX) result = -1; /* TODO: use errno! */
    else at_exit[at_exit_count++] = func;
    (void)sys_release(&at_exit_lock);
  }
  else result = -1; /* TODO: use errno! */
  return result;
}

void exit(int status)
{
  while (sys_acquire(&at_exit_lock)) sys_switch();
  while (at_exit_count--) at_exit[at_exit_count]();
  sys_group_exit(status);
}

