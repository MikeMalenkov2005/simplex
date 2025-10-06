#include <stdlib.h>

static void swap(char *p1, char *p2, size_t size)
{
  char tmp;
  do
  {
    tmp = *p1;
    *p1++ = *p2;
    *p2++ = tmp;
  }
  while (--size);
}

static const char *part(const char *low, const char *high, size_t size, comparator_t compar)
{
  const char *p = high;
  const char *i = low;
  const char *j;
  for (j = low; j <= high; j += size)
  {
    if (compar(j, p) <= 0)
    {
      swap((char*)i, (char*)j, size);
      i += size;
    }
  }
  swap((char*)i, (char*)high, size);
  return i;
}

static void sort(const char *low, const char *high, size_t size, comparator_t compar)
{
  const char *p;
  if (low < high)
  {
    p = part(low, high, size, compar);
    sort(low, p - size, size, compar);
    sort(p, high, size, compar);
  }
}

void qsort(const void *base, size_t nmemb, size_t size, comparator_t compar)
{
  if (base && nmemb && size && compar) sort(base, (const char*)base + (nmemb - 1) * size, size, compar);
}

