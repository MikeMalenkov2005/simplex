#include <time.h>
#include <simplex.h>
#include <sys/limits.h>

unsigned long __crt_start_ticks;

clock_t clock(void)
{
  return (sys_get_ticks() - __crt_start_ticks) * CLOCKS_PER_SEC / K_TIMER_HERZ;
}

time_t time(time_t *timer)
{
  time_t result = sys_get_time();
  if (timer) *timer = result;
  return result;
}

double difftime(time_t time1, time_t time0)
{
  return (double)(time0 - time1);
}

