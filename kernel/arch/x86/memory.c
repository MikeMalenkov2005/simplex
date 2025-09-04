#include "mmu.h"
#include "utils.h"

extern void __end;

K_BOOL MMU_Init()
{
  K_USIZE *table, *dir = (K_USIZE*)K_FirstFreePage;
  K_USIZE page = MMU_PAGE_PRESENT | MMU_PAGE_READABLE | MMU_PAGE_WRITABLE | MMU_PAGE_EXECUTABLE | MMU_PAGE_GLOBAL;
  K_USIZE pti = 0, pdi = 0;
  if (K_FreePageCount < (K_PageUp(&__end) >> K_PAGE_SHIFT >> MMU_INDEX_BITS) + 2) return FALSE;
  K_FirstFreePage = *dir;
  --K_FreePageCount;
  while (pdi < MMU_INDEX_MASK) dir[pdi++] = 0;
  dir[pdi] = (K_USIZE)dir | MMU_PAGE_PRESENT | MMU_PAGE_WRITABLE;
  pdi = 0;
  while (page < K_PageUp(&__end))
  {
    if (!pti)
    {
      table = (K_USIZE*)K_FirstFreePage;
      dir[pdi++] = K_FirstFreePage | MMU_PAGE_PRESENT | MMU_PAGE_WRITABLE | MMU_PAGE_USER_MODE;
      K_FirstFreePage = *table;
      --K_FreePageCount;
      (void)K_ZeroMemory(table, K_PAGE_SIZE);
    }
    table[pti++] = page;
    if (pti > MMU_INDEX_MASK)
    {
      dir[pdi - 1] |= MMU_PAGE_GLOBAL;
      pti = 0;
    }
    page += K_PAGE_SIZE;
  }
  K_SetPageMap(dir);
  MMU_Enable();
  return TRUE;
}

K_HANDLE K_CreatePageMap()
{
  if (K_FreePageCount < 2) return NULL;
  return NULL; /* TODO: IMPLEMENT */
}

K_BOOL K_DeletePageMap(K_HANDLE map)
{
  if (K_GetPageMap() == map) return FALSE;
  (void)map;
  return FALSE; /* TODO: IMPLEMENT */
}

K_USIZE K_GetPage(K_HANDLE address)
{
  K_USIZE pti, pdi, pte, page = 0;
  if (K_IsPageTable(address)) return 0;
  pti = (K_USIZE)address >> K_PAGE_SHIFT;
  pdi = pti >> MMU_INDEX_BITS;
  if (((K_USIZE*)MMU_PAGE_DIR)[pdi] & MMU_PAGE_PRESENT)
  {
    pte = ((volatile K_USIZE*)MMU_PAGE_TABLE)[pti];
    page = pte & K_PAGE_ADDRESS_MASK;
    if (pte & MMU_PAGE_PRESENT) page |= K_PAGE_VALID;
    if (pte & MMU_PAGE_READABLE) page |= K_PAGE_READABLE;
    if (pte & MMU_PAGE_WRITABLE) page |= K_PAGE_WRITABLE;
    if (pte & MMU_PAGE_EXECUTABLE) page |= K_PAGE_EXECUTABLE;
    if (pte & MMU_PAGE_USER_MODE) page |= K_PAGE_USER_MODE;
    if (pte & MMU_PAGE_GLOBAL) page |= K_PAGE_GLOBAL;
    if (pte & MMU_PAGE_EXTERNAL) page |= K_PAGE_EXTERNAL;
    if (pte & MMU_PAGE_CACHE_DISABLE) page |= K_PAGE_CACHE_DISABLE;
  }
  return page;
}

K_BOOL K_SetPage(K_HANDLE address, K_USIZE page)
{
  K_USIZE pti, pdi, pte;
  if (K_IsPageTable(address)) return FALSE;
  pti = (K_USIZE)address >> K_PAGE_SHIFT;
  pdi = pti >> MMU_INDEX_BITS;
  if (!(((K_USIZE*)MMU_PAGE_DIR)[pdi] & MMU_PAGE_PRESENT))
  {
    if (!K_FreePageCount) return FALSE;
    ((volatile K_USIZE*)MMU_PAGE_DIR)[pdi] = K_FirstFreePage | MMU_PAGE_PRESENT | MMU_PAGE_WRITABLE | MMU_PAGE_USER_MODE;
    pte = (MMU_PAGE_TABLE | (pdi << K_PAGE_SHIFT));
    K_InvalidatePage((K_HANDLE)pte);
    K_FirstFreePage = *(volatile K_USIZE*)pte;
    --K_FreePageCount;
    (void)K_ZeroMemory((void*)pte, K_PAGE_SIZE);
  }
  pte = page & K_PAGE_ADDRESS_MASK;
  if (page & K_PAGE_VALID) pte |= MMU_PAGE_PRESENT;
  if (page & K_PAGE_READABLE) pte |= MMU_PAGE_READABLE;
  if (page & K_PAGE_WRITABLE) pte |= MMU_PAGE_WRITABLE;
  if (page & K_PAGE_EXECUTABLE) pte |= MMU_PAGE_EXECUTABLE;
  if (page & K_PAGE_USER_MODE) pte |= MMU_PAGE_USER_MODE;
  if (page & K_PAGE_GLOBAL) pte |= MMU_PAGE_GLOBAL;
  if (page & K_PAGE_EXTERNAL) pte |= MMU_PAGE_EXTERNAL;
  if (page & K_PAGE_CACHE_DISABLE) pte |= MMU_PAGE_CACHE_DISABLE;
  ((volatile K_USIZE*)MMU_PAGE_TABLE)[pti] = page;
  K_InvalidatePage(address);
  for (pti = 0; pti <= MMU_INDEX_MASK; ++pti)
  {
    if (((volatile K_USIZE*)MMU_PAGE_TABLE)[(pdi << MMU_INDEX_BITS) | pti]) return TRUE;
  }
  ((volatile K_USIZE*)MMU_PAGE_TABLE)[pdi << MMU_INDEX_BITS] = K_FirstFreePage;
  K_FirstFreePage = ((volatile K_USIZE*)MMU_PAGE_DIR)[pdi] & K_PAGE_ADDRESS_MASK;
  ((volatile K_USIZE*)MMU_PAGE_DIR)[pdi] = 0;
  ++K_FreePageCount;
  K_InvalidatePage(&((K_USIZE*)MMU_PAGE_TABLE)[pdi << MMU_INDEX_BITS]);
  return TRUE;
}

K_BOOL K_IsPageTable(K_HANDLE address)
{
  return (K_USIZE)address >= MMU_PAGE_TABLE;
}

