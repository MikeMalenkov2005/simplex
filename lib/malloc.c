#include <string.h>

#include "crt.h"

typedef struct block_t block_t;

struct block_t
{
  block_t *next;
  block_t *prev;
  size_t size;
  size_t free;
};

#define ALIGN(val)  (((size_t)(val) + 15) & ~(size_t)15)

static block_t *heap = NULL;
static int heap_lock = 0;

static block_t *find_free_block(size_t size)
{
  block_t *block = heap;
  while (block && block->size < size && !block->free)
  {
    block = block->next;
  }
  return block;
}

static void heap_defrag(block_t *block)
{
  block_t *swap;
  if (block->free)
  {
    while (block != heap && block->prev->free)
    {
      block = block->prev;
    }
    while (block->next && block->next->free)
    {
      block->size += block->next->size + sizeof *block;
      block->next = (swap = block->next)->next;
      swap->prev = NULL;
      swap->next = NULL;
      swap->size = 0;
      swap->free = 0;
    }
    if (!block->next) heap->prev = block;
    else block->next->prev = block;
  }
}

static void heap_split(block_t *block, size_t size) /* Assumes size is 16 byte aligned and not zero. */
{
  block_t *second;
  if (block->free && block->size > size + sizeof *block)
  {
    second = (void*)(block + 1) + size;
    second->prev = block;
    second->next = block->next;
    second->size = block->size - size - sizeof *block;
    second->free = 1;
    block->next = second;
    block->size = size;
    if (block == heap) block->prev = second;
  }
}

static void heap_free(void *ptr)
{
  block_t *block = ptr - sizeof *block;
  if (ptr && !((size_t)ptr & 15) && block->prev && block->size && !(block->size & 15) && !block->free)
  {
    block->free = 1;
    heap_defrag(block);
  }
}

static void *heap_alloc(size_t size)
{
  block_t *block;
  if (!(size = ALIGN(size))) return NULL;
  if (!(block = find_free_block(size)))
  {
    if (heap && heap->prev->free)
    {
      block = heap->prev;
      if (__crt_sbrk(size - block->size) == SBRK_ERR) return NULL;
    }
    else
    {
      block = __crt_sbrk(size + sizeof *block);
      if (block == SBRK_ERR) return NULL;
      if (heap)
      {
        block->prev = heap->prev;
        block->prev->next = heap->prev = block;
      }
      else heap = block->prev = block;
      block->next = NULL;
    }
    block->size = size;
  }
  heap_split(block, size);
  block->free = 0;
  return block + 1;
}

static void *heap_realloc(void *ptr, size_t size)
{
  void *result = NULL;
  block_t *block = ptr - sizeof *block;
  
  if (!(size = ALIGN(size))) return NULL;
  if (!ptr) return heap_alloc(size);
  
  if (!((size_t)ptr & 15) && block->prev && block->size && !(block->size & 15) && !block->free)
  {
    /* TODO: improve this function! */
    if ((result = heap_alloc(size)))
    {
      memmove(result, ptr, block->size > size ? size : block->size);
      heap_free(ptr);
    }
  }
  return result;
}

void *realloc(void *ptr, size_t size)
{
  void *result;
  __crt_lock(&heap_lock);
  result = heap_realloc(ptr, size);
  __crt_unlock(&heap_lock);
  return result;
}

void *malloc(size_t size)
{
  void *result;
  __crt_lock(&heap_lock);
  result = heap_alloc(size);
  __crt_unlock(&heap_lock);
  return result;
}

void *calloc(size_t nmemb, size_t size)
{
  size_t total = nmemb * size;
  void *result = malloc(total);
  return result ? memset(result, 0, total) : result;
}

void free(void *ptr)
{
  __crt_lock(&heap_lock);
  heap_free(ptr);
  __crt_unlock(&heap_lock);
}

