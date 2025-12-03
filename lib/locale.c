#include <locale.h>
#include <limits.h>

/* TODO: Add locales other then 'C' */

char *setlocale(int category, const char *locale)
{
  static char result[2];
  if (category < LC_ALL || category > LC_TIME) return NULL;
  if (locale && (locale[0] != 'C' || locale[1])) return NULL;
  result[0] = 'C';
  result[1] = 0;
  return result;
}

struct lconv *localeconv(void)
{
  static struct lconv conv;
  static char tmp[2];
  tmp[0] = '.';
  tmp[1] = 0;
  conv = (struct lconv)
  {
    tmp,
    tmp + 1,
    tmp + 1,
    tmp + 1,
    tmp + 1,
    tmp + 1,
    tmp + 1,
    tmp + 1,
    tmp + 1,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    tmp + 1,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
    CHAR_MAX,
  };
  return &conv;
}

