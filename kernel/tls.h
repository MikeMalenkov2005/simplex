#ifndef _K_TLS_H
#define _K_TLS_H

#include <sys/types.h>
#include <sys/limits.h>

#define K_TLS_INVALID_INDEX   (~(K_USIZE)0)
#define K_TLS_SLOT_COUNT_MAX  ((K_PAGE_SIZE - sizeof(K_HANDLE)) / sizeof(K_USIZE) - 1)

typedef struct K_TLS K_TLS;

struct K_TLS
{
  K_TLS *pParent;
  K_USIZE SlotCount;
  K_USIZE Slots[K_TLS_SLOT_COUNT_MAX];
};

K_TLS *K_CreateTLS(K_TLS *parent);

void K_DeleteTLS(K_TLS *tls);

K_USIZE K_TLSNewEntry(K_TLS *tls);

K_USIZE K_TLSGetEntry(K_TLS *tls, K_USIZE index);

K_BOOL K_TLSSetEntry(K_TLS *tls, K_USIZE index, K_USIZE value);

#endif

