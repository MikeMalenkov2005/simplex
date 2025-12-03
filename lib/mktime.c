#include <time.h>
#include <stdio.h>
#include <simplex.h>

#define EPOCH_YEAR          70  /* since 1900 */
#define EPOCH_YEAR_REAL     1970
#define LEAPS_BEFORE_EPOCH  477

int fixdst(struct tm *timeptr)
{
  /* TODO: Calculate DST from time if < 0 */
  return (timeptr->tm_isdst > 0);
}

time_t mktime(struct tm *timeptr)
{
  if (!timeptr) return (time_t)-1;
  long days = timeptr->tm_mday - 1;
  int hour = timeptr->tm_hour - fixdst(timeptr);
  int year = timeptr->tm_year + 1900, month = timeptr->tm_mon + 1;
  int leap = (!(year % 4) && (year % 100)) || !(year % 400);
  int leaps = (year / 4 - year / 100 + year / 400) - LEAPS_BEFORE_EPOCH - leap;

  if (timeptr->tm_sec > 60 || timeptr->tm_sec < 0) return (time_t)-1;
  if (timeptr->tm_min > 59 || timeptr->tm_min < 0) return (time_t)-1;
  if (hour > 23 || hour < 0) return (time_t)-1;
  if (days > 30 || days < 0) return (time_t)-1;
  if (month > 12 || month < 1) return (time_t)-1;

  if (month > 1) days += 31;
  if (month > 2) days += 28 + leap;
  if (month > 3) days += 31;
  if (month > 4) days += 30;
  if (month > 5) days += 31;
  if (month > 6) days += 30;
  if (month > 7) days += 31;
  if (month > 8) days += 31;
  if (month > 9) days += 30;
  if (month > 10) days += 31;
  if (month > 11) days += 30;
  
  timeptr->tm_yday = (int)days;
  days += (time_t)(year - EPOCH_YEAR_REAL) * 365 + leaps;
  if ((timeptr->tm_wday = (int)((days + 4) % 7)) < 0) timeptr->tm_wday += 7;

  return days * 86400 + hour * 3600 + timeptr->tm_min * 60
    + timeptr->tm_sec - sys_get_zone();
}

char *asctime(const struct tm *timeptr)
{
  static const char wday_name[7][4] =
  {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  static const char mon_name[12][4] =
  {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  static char result[32];

  if (!timeptr) return NULL;
  if (timeptr->tm_sec > 60 || timeptr->tm_sec < 0) return NULL;
  if (timeptr->tm_min > 59 || timeptr->tm_min < 0) return NULL;
  if (timeptr->tm_hour > 60 || timeptr->tm_hour < 0) return NULL;

  sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
      wday_name[timeptr->tm_wday], mon_name[timeptr->tm_mon],
      timeptr->tm_mday, timeptr->tm_hour, timeptr->tm_min,
      timeptr->tm_sec, timeptr->tm_year + 1900);

  return result;
}

char *ctime(const time_t *timer)
{
  return asctime(localtime(timer));
}

struct tm *gmtime(const time_t *timer)
{
  static struct tm result;
  long value = *timer;
  int check, leap;
    
  if ((result.tm_sec = value % 60) < 0) result.tm_sec += 60;
  value /= 60;
  if ((result.tm_min = value % 60) < 0) result.tm_min += 60;
  value /= 60;
  if ((result.tm_hour = value % 24) < 0) result.tm_hour += 24;
  value /= 24;
  if ((result.tm_wday = (int)(value + 4) % 7) < 0) result.tm_wday += 7;

  result.tm_year = EPOCH_YEAR;
  check = (1900 + EPOCH_YEAR) % 400;
  leap = 0;
  while (value >= 365 + leap)
  {
    value -= 365 + leap;
    if (++check == 400) check = 0;
    leap = !(check & 3) && check != 100 && check != 200 && check != 300;
    ++result.tm_year;
  }
  while (value < 0)
  {
    value += 365 + leap;
    if (!check--) check = 399;
    leap = !(check & 3) && check != 100 && check != 200 && check != 300;
    --result.tm_year;
  }
  result.tm_yday = (int)value;
  
  result.tm_mon = 0;
  result.tm_mday = (int)value + 1;
  if (result.tm_mon == 0 && result.tm_mday > 31) ++result.tm_mon, result.tm_mday -= 31;
  if (result.tm_mon == 1 && result.tm_mday > 28 + leap) ++result.tm_mon, result.tm_mday -= 28 + leap;
  if (result.tm_mon == 2 && result.tm_mday > 31) ++result.tm_mon, result.tm_mday -= 31;
  if (result.tm_mon == 3 && result.tm_mday > 30) ++result.tm_mon, result.tm_mday -= 30;
  if (result.tm_mon == 4 && result.tm_mday > 31) ++result.tm_mon, result.tm_mday -= 31;
  if (result.tm_mon == 5 && result.tm_mday > 30) ++result.tm_mon, result.tm_mday -= 30;
  if (result.tm_mon == 6 && result.tm_mday > 31) ++result.tm_mon, result.tm_mday -= 31;
  if (result.tm_mon == 7 && result.tm_mday > 31) ++result.tm_mon, result.tm_mday -= 31;
  if (result.tm_mon == 8 && result.tm_mday > 30) ++result.tm_mon, result.tm_mday -= 30;
  if (result.tm_mon == 9 && result.tm_mday > 31) ++result.tm_mon, result.tm_mday -= 31;
  if (result.tm_mon == 10 && result.tm_mday > 30) ++result.tm_mon, result.tm_mday -= 30;
  result.tm_isdst = -1;

  return &result;
}

struct tm *localtime(const time_t *timer)
{
  const time_t local = *timer + sys_get_zone();
  return gmtime(&local);
}

