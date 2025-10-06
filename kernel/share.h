#ifndef _K_SHARE_H
#define _K_SHARE_H

#include <sys/types.h>
#include <sys/limits.h>

typedef struct K_ShareInfo
{
  K_HANDLE Source;
  K_HANDLE Mirror;
  K_USIZE Target;
  K_USIZE Size;
} K_ShareInfo;

#define K_MAX_SHARE_INFO_COUNT  (K_PAGE_SIZE / sizeof(K_ShareInfo))

#define K_SHARE_INFO_SIZE (K_MAX_SHARE_INFO_COUNT * sizeof(K_ShareInfo))

K_ShareInfo *K_CreateShareInfo();

void K_DeleteShareInfo(K_ShareInfo *info);

K_HANDLE K_ShareMemory(K_USIZE target, K_HANDLE address, K_USIZE size);

#define K_UnshareMemory(target, address) (!!K_ShareMemory(target, address, 0))

#endif

