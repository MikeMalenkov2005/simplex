#include <stdlib.h>

void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, comparator_t compar)
{
  size_t low = 0, high = nmemb, mid;
  void *ptr;
  int diff;

  if (!key || !base || !nmemb || !size || !compar) return NULL;

  while (low < high)
  {
    mid = (low + high) >> 1;
    ptr = (char*)base + mid * size;
    diff = compar(key, ptr);

    if (diff == 0) return ptr;
    if (diff > 0) low = mid + 1;
    if (diff < 0) high = mid;
  }
  return NULL;
}

