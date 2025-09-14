#include <string.h>
#include <simplex.h>
#include <sys/memory.h>
#include <sys/limits.h>

/*

struct free_t
{
  struct free_t *next;
  size_t size;
};

typedef struct free_t free_t;

extern void * __crt_heap_base;
extern size_t __crt_heap_size;

static size_t heap_commited = 0;
static free_t *first_free = NULL;
static int heap_lock = -1;

void *realloc(void *ptr, size_t size)
{
  free_t *blk;
  void *result = NULL;
  size = (size + 15) & ~(size_t)15;
  while (sys_acquire(&heap_lock)) sys_switch();

  if (!first_free && !sys_remap(__crt_heap_base, size + sizeof *blk, MAP_READ | MAP_WRITE))
  {
    first_free = __crt_heap_base;
    first_free->size = ((size + sizeof *blk) + K_PAGE_SIZE - 1) & (K_PAGE_SIZE - 1);
  }
  if ((blk = first_free)) while (blk->next && blk->size < size) blk = blk->next;

  if (blk && blk->size >= size)

  (void)sys_release(&heap_lock);
  return result;
}

void *malloc(size_t size)
{
  return realloc(NULL, size);
}

void *calloc(size_t nmemb, size_t size)
{
  size_t total = nmemb * size;
  void *result = malloc(total);
  return result ? memset(result, 0, total) : result;
}

void free(void *ptr)
{
  size_t *size = ptr - sizeof(size_t);
  if (ptr)
  {
  }
}

*/

