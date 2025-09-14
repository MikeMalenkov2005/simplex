#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

int memcmp(const void *lhs, const void *rhs, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, int ch, size_t count);

#endif

