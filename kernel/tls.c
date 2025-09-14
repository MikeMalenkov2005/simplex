#include "tls.h"
#include "utils.h"
#include "memory.h"

K_TLS *K_CreateTLS(K_TLS *parent)
{
  K_TLS *tls = K_FindLastFreeAddress(sizeof *tls);
  if (tls && K_AllocatePages(tls, sizeof *tls, K_PAGE_WRITABLE | K_PAGE_READABLE))
  {
    if (parent) *tls = *parent;
    else tls = K_ZeroStruct(tls);
  }
  else tls = NULL;
  return tls;
}

void K_DeleteTLS(K_TLS *tls)
{
  if (tls) K_FreePages(tls, sizeof *tls);
}

K_USIZE K_TLSNewEntry(K_TLS *tls)
{
  K_USIZE *count;
  if (!tls) return K_TLS_INVALID_INDEX;
  count = tls->pParent ? &tls->pParent->SlotCount : &tls->SlotCount;
  if (*count >= K_TLS_SLOT_COUNT_MAX) return K_TLS_INVALID_INDEX;
  return (*count)++;
}

K_USIZE K_TLSGetEntry(K_TLS *tls, K_USIZE index)
{
  K_USIZE count;
  if (!tls) return 0;
  count = tls->pParent ? tls->pParent->SlotCount : tls->SlotCount;
  if (index >= count) return 0;
  return tls->Slots[index];
}

K_BOOL K_TLSSetEntry(K_TLS *tls, K_USIZE index, K_USIZE value)
{
  K_USIZE count;
  if (!tls) return FALSE;
  count = tls->pParent ? tls->pParent->SlotCount : tls->SlotCount;
  if (index >= count) return FALSE;
  tls->Slots[index] = value;
  return TRUE;
}

