#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>

/* EXIT */

#define EXIT_SUCCESS  0
#define EXIT_FAILURE  1

void abort(void);

int atexit(void (*func)(void));

void exit(int status);

/* RAND */

#define RAND_MAX  0x7FFF

int rand(void);

void srand(unsigned int seed);

/* HEAP */

void *calloc(size_t nmemb, size_t size);

void *malloc(size_t size);

void *realloc(void *ptr, size_t size);

void free(void *ptr);

/* ARR */

typedef int (*comparator_t)(const void *, const void *);

void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, comparator_t compar);

void qsort(const void *base, size_t nmemb, size_t size, comparator_t compar);

/* ENV */

char *getenv(const char *name);

int system(const char *string);

/* MATH */

struct div_t
{
  int quot;
  int rem;
};

typedef struct div_t div_t;

struct ldiv_t
{
  long quot;
  long rem;
};

typedef struct ldiv_t ldiv_t;

int abs(int j);

div_t div(int numer, int denom);

long labs(long j);

ldiv_t ldiv(long numer, long denom);

#endif

