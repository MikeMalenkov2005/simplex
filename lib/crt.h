#ifndef __CRT_H
#define __CRT_H

#include <stddef.h>

#define SBRK_ERR  ((void*)-1)

struct __crt_info
{
  int __crt_errno;
};

int __crt_lock(int *lock);
void __crt_unlock(int *lock);

int __crt_brk(void *addr);
void *__crt_sbrk(ptrdiff_t incr);

struct __crt_info *__crt_get_info(void);

#endif

