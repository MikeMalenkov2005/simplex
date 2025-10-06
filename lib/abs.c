#include <limits.h>
#include <stdlib.h>

int abs(int j)
{
  const int mask = j >> (sizeof j * CHAR_BIT - 1);
  return (j + mask) ^ mask;
}

long labs(long j)
{
  const long mask = j >> (sizeof j * CHAR_BIT - 1);
  return (j + mask) ^ mask;
}

