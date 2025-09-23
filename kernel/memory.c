#include "memory.h"
#include "types.h"

extern void __end;

K_USIZE K_FirstFreePage;
K_USIZE K_FreePageCount;

K_BOOL K_AllocatePage(K_HANDLE address, K_U16 flags)
{
  address = (K_HANDLE)K_PageDown(address); /* THIS WAS THE ULTIMATE FIX >:( */
  if (K_GetPage(address) || !K_SetPage(address, K_PAGE_EXTERNAL)) return FALSE;
  if (!K_IsPageCommited(flags)) return K_SetPage(address, flags & K_PAGE_FLAGS_MASK & ~K_PAGE_VALID);
  if (!K_FreePageCount) return !K_SetPage(address, 0);
  (void)K_SetPage(address, K_FirstFreePage | (flags & K_PAGE_FLAGS_MASK) | K_PAGE_VALID);
  K_FirstFreePage = *(K_USIZE*)address;
  --K_FreePageCount;
  return TRUE;
}

K_BOOL K_FreePage(K_HANDLE address)
{
  K_USIZE page;
  address = (K_HANDLE)K_PageDown(address);
  if (K_IsPageTable(address)) return FALSE;
  page = K_GetPage(address);
  if (!K_IsPageCommited(page)) return K_SetPage(address, 0);
  if (!(page & K_PAGE_VALID)) return FALSE;
  *(volatile K_USIZE*)address = K_FirstFreePage;
  K_FirstFreePage = page & K_PAGE_ADDRESS_MASK;
  ++K_FreePageCount;
  return K_SetPage(address, 0);
}

K_BOOL K_ChangePage(K_HANDLE address, K_U16 flags)
{
  K_USIZE page = K_GetPage(address);
  flags &= K_PAGE_FLAGS_MASK & ~K_PAGE_VALID;
  if (!page || ((page ^ flags) & K_PAGE_EXTERNAL)) return FALSE;
  if ((page & K_PAGE_FLAGS_MASK & ~K_PAGE_VALID) == flags) return TRUE;
  if (!(page & K_PAGE_EXTERNAL))
  {
    if (K_IsPageCommited(page) && !K_IsPageCommited(flags))
    {
      return K_FreePage(address) && K_SetPage(address, flags); /* Decommit page */
    }
    if (!K_IsPageCommited(page) && K_IsPageCommited(flags))
    {
      return K_SetPage(address, 0) && K_AllocatePage(address, flags); /* Commit page */
    }
  }
  return K_SetPage(address, (page & (K_PAGE_ADDRESS_MASK | K_PAGE_VALID)) | flags);
}

K_HANDLE K_FindFirstFreeAddress(K_USIZE size)
{
  K_USIZE offset;
  K_USIZE address = K_PageUp(&__end);
  K_BOOL found = FALSE;
  size = K_PageUp(size);
  if (!size) return NULL;
  while (!found && address)
  {
    found = TRUE;
    for (offset = 0; found && offset < size ; offset += K_PAGE_SIZE)
    {
      if (!K_IsAddressFree((K_HANDLE)(address + offset)))
      {
        found = FALSE;
        address += K_PAGE_SIZE;
      }
    }
  }
  return (K_HANDLE)address;
}

K_HANDLE K_FindLastFreeAddress(K_USIZE size)
{
  K_USIZE offset;
  K_USIZE address = K_PAGE_ADDRESS_MASK;
  K_BOOL found = FALSE;
  if (!size) return NULL;
  while (!found && address)
  {
    found = TRUE;
    for (offset = 0; found && offset < size ; offset += K_PAGE_SIZE)
    {
      if (!K_IsAddressFree((K_HANDLE)(address + offset)))
      {
        found = FALSE;
        address -= K_PAGE_SIZE;
      }
    }
  }
  return (K_HANDLE)address;
}

K_U16 K_GetRangeFlags(K_HANDLE address, K_USIZE size)
{
  K_USIZE offset;
  K_U16 flags = (K_GetPage(address) & K_PAGE_FLAGS_MASK) | K_PAGE_VALID;
  for (offset = K_PAGE_SIZE; offset < size; offset += K_PAGE_SIZE)
  {
    if (K_IsPageTable(address + offset)) return 0;
    if (((K_GetPage(address + offset) & K_PAGE_FLAGS_MASK) | K_PAGE_VALID) != flags) return 0;
  }
  return flags;
}

K_BOOL K_AllocatePages(K_HANDLE address, K_USIZE size, K_U16 flags)
{
  K_USIZE offset;
  for (offset = 0; offset < size; offset += K_PAGE_SIZE)
  {
    if (!K_AllocatePage(address + offset, flags))
    {
      size = offset;
      for (offset = 0; offset < size; offset += K_PAGE_SIZE)
      {
        (void)K_FreePage(address + offset);
      }
      return FALSE;
    }
  }
  return TRUE;
}

K_BOOL K_FreePages(K_HANDLE address, K_USIZE size)
{
  K_USIZE offset;
  if (!K_GetRangeFlags(address, size)) return FALSE;
  for (offset = 0; offset < size; offset += K_PAGE_SIZE)
  {
    (void)K_FreePage(address + offset);
  }
  return TRUE;
}

K_BOOL K_ChangePages(K_HANDLE address, K_USIZE size, K_U16 flags)
{
  K_USIZE offset;
  K_U16 old = K_GetRangeFlags(address, size);
  if (!old) return FALSE;
  if (old == (flags | K_PAGE_VALID)) return TRUE;
  for (offset = 0; offset < size; offset += K_PAGE_SIZE)
  {
    if (!K_ChangePage(address + offset, flags))
    {
      size = offset;
      for (offset = 0; offset < size; offset += K_PAGE_SIZE)
      {
        (void)K_ChangePage(address + offset, old);
      }
      return FALSE;
    }
  }
  return TRUE;
}

K_BOOL K_IsUserRange(K_HANDLE address, K_USIZE size, K_U16 filter)
{
  K_USIZE offset;
  K_BOOL result = TRUE;
  K_U16 flags;
  if (!(size = K_PageUp(size))) result = FALSE;
  for (offset = 0; result && offset < size; offset += K_PAGE_SIZE)
  {
    flags = K_GetPage(address + offset);
    result = (flags & K_PAGE_USER_MODE) && (flags & filter) == filter;
  }
  return result;
}

