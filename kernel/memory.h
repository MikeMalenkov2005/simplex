#ifndef _K_MEMORY_H
#define _K_MEMORY_H

#include <types.h>
#include <sys/limits.h>

#define K_PAGE_FLAGS_MASK   (K_PAGE_SIZE - 1)
#define K_PAGE_ADDRESS_MASK (~(K_USIZE)K_PAGE_FLAGS_MASK)

#define K_PageDown(p) ((K_USIZE)(p) & K_PAGE_ADDRESS_MASK)
#define K_PageUp(p)   K_PageDown((K_USIZE)(p) + K_PAGE_FLAGS_MASK)

#define K_PAGE_VALID          (1 << 0)
#define K_PAGE_READABLE       (1 << 1)
#define K_PAGE_WRITABLE       (1 << 2)
#define K_PAGE_EXECUTABLE     (1 << 3)
#define K_PAGE_USER_MODE      (1 << 4)
#define K_PAGE_GLOBAL         (1 << 5)
#define K_PAGE_EXTERNAL       (1 << 6)
#define K_PAGE_CACHE_DISABLE  (1 << 7)

#define K_IsPageCommited(p) ((p) & (K_PAGE_READABLE | K_PAGE_WRITABLE | K_PAGE_EXECUTABLE))

K_HANDLE K_GetPageMap();
void K_SetPageMap(K_HANDLE map);

K_HANDLE K_CreatePageMap();
K_BOOL K_DeletePageMap(K_HANDLE map);

void K_InvalidatePage(K_HANDLE address);

K_USIZE K_GetPage(K_HANDLE address);
K_BOOL K_SetPage(K_HANDLE address, K_USIZE page);

K_BOOL K_IsPageTable(K_HANDLE address);

K_BOOL K_AllocatePage(K_HANDLE address, K_U16 flags);
K_BOOL K_FreePage(K_HANDLE address);
K_BOOL K_ChangePage(K_HANDLE address, K_U16 flags);

#define K_IsAddressFree(address) (!K_IsPageTable(address) && !K_GetPage(address))

K_HANDLE K_FindFirstFreeAddress(K_USIZE size);
K_HANDLE K_FindLastFreeAddress(K_USIZE size);

K_U16 K_GetRangeFlags(K_HANDLE address, K_USIZE size);

K_BOOL K_AllocatePages(K_HANDLE address, K_USIZE size, K_U16 flags);
K_BOOL K_FreePages(K_HANDLE address, K_USIZE size);
K_BOOL K_ChangePages(K_HANDLE address, K_USIZE size, K_U16 flags);

K_BOOL K_IsUserRange(K_HANDLE address, K_USIZE size);

#endif

