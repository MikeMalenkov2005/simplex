#ifndef _STDIO_H
#define _STDIO_H

#include <limits.h>

typedef struct FILE FILE;

struct fpos_t
{
#if ULONG_MAX > UINT_MAX
  unsigned long value;
#else
  unsigned long low;
  unsigned long high;
#endif
};

typedef struct fpos_t fpos_t;

#define EOF (-1)

#endif

