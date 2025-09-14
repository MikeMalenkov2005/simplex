#include <signal.h>
#include <simplex.h>
#include <sys/syscall.h>

#define NSIG  16

static sighandler_t sigtable[NSIG] = { SIG_DFL };

sighandler_t signal(int sig, sighandler_t func)
{
  sighandler_t old = SIG_ERR;
  if (sig >= 0 && sig < NSIG)
  {
    old = sigtable[sig];
    sigtable[sig] = func;
  }
  return old;
}

int raise(int sig)
{
  if (sig >= 0 && sig < NSIG)
  {
    if (sigtable[sig] == SIG_DFL) switch (sig)
    {
    }
    if (sigtable[sig] != SIG_IGN) sigtable[sig](sig);
    return 0;
  }
  return -1; /* TODO: USE ERRNO WITH EINVAL */
}

