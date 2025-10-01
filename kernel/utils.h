#ifndef _K_UTILS_H
#define _K_UTILS_H

#include <sys/types.h>

void *K_ZeroMemory(void *memory, K_USIZE size);

#define K_ZeroStruct(p) K_ZeroMemory(p, sizeof *(p))

void *memcpy(void *dest, void *src, K_USIZE size);

#endif

