#include "crt.h"
#include "stddef.h"

#define __CRT_MAP_FLAGS

#include <string.h>
#include <stdlib.h>
#include <simplex.h>
#include <sys/memory.h>
#include <sys/limits.h>

extern void __crt_signal_handler(void);

static long __crt_info_tls;

static void *__crt_brk_base;
static void *__crt_brk_value;
static void *__crt_brk_limit;
static int __crt_brk_lock = 0;

void __crt_init(void)
{
  (void)sys_handler(__crt_signal_handler);
  if ((__crt_info_tls = sys_tls_new()) == -1) abort();
  if (!(__crt_brk_base = sys_map(NULL, K_PAGE_SIZE, MAP_RD | MAP_WR | MAP_UP))) abort();
  __crt_brk_limit = (__crt_brk_value = __crt_brk_base) + K_PAGE_SIZE;
}

void __crt_lock(int *lock)
{
  while (__crt_atomic_swap(lock, 1)) sys_switch();
}

void __crt_unlock(int *lock)
{
  (void)__crt_atomic_swap(lock, 0);
}

static void *__crt_sbrk_internal(ptrdiff_t incr)
{
  ptrdiff_t size;
  void *result, *temp;
  if (incr >= 0) result = __crt_brk_base;
  else result = (__crt_brk_base - __crt_brk_value > incr) ? SBRK_ERR : NULL;
  if (__crt_brk_limit - __crt_brk_value > incr)
  {
    size = __crt_brk_value - __crt_brk_limit + incr;
    size = (size + K_PAGE_SIZE - 1) & ~(ptrdiff_t)(K_PAGE_SIZE - 1);
    temp = sys_map(NULL, size, MAP_RD | MAP_WR | MAP_UP);
    if (temp != __crt_brk_limit)
    {
      (void)sys_free(temp, size);
      result = SBRK_ERR;
    }
    else __crt_brk_limit += size;
  }
  __crt_brk_value += incr;
  return result;
}

int __crt_brk(void *addr)
{
  int failed;
  __crt_lock(&__crt_brk_lock);
  failed = __crt_sbrk_internal(addr - __crt_brk_value) == SBRK_ERR;
  __crt_unlock(&__crt_brk_lock);
  return -failed;
}

void *__crt_sbrk(ptrdiff_t incr)
{
  void *result;
  __crt_lock(&__crt_brk_lock);
  result = __crt_sbrk_internal(incr);
  __crt_unlock(&__crt_brk_lock);
  return result;
}

struct __crt_info *__crt_get_info(void)
{
  struct __crt_info *info = sys_tls_get(__crt_info_tls);
  if (!info)
  {
    if ((info = malloc(sizeof *info)))
    {
      (void)sys_tls_set(__crt_info_tls, memset(info, 0, sizeof *info));
    }
    else abort(); /* TODO: Abort only the calling thread! */
  }
  return info;
}

