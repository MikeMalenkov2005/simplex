#include "share.h"

#include "memory.h"
#include "utils.h"
#include "task.h"

static void K_ClearShareInfo(volatile K_ShareInfo *info)
{
  K_Task *target = K_GetMainTask(info->Target);
  K_Task *current = K_GetCurrentTask();
  K_HANDLE mirror = info->Mirror;
  K_USIZE offset, size = info->Size + ((K_USIZE)mirror & K_PAGE_FLAGS_MASK);

  if (target && target->PageMap != current->PageMap)
  {
    K_SetPageMap(target->PageMap);
    for (offset = 0; offset < size; offset += K_PAGE_SIZE)
    {
      if (K_GetPage(mirror + offset) & K_PAGE_EXTERNAL)
      {
        (void)K_SetPage(mirror + offset, 0);
      }
    }
    K_SetPageMap(current->PageMap);
  }
  info->Source = NULL;
  info->Mirror = NULL;
  info->Target = 0;
  info->Size = 0;
}

static K_HANDLE K_ApplyShareInfo(volatile K_ShareInfo *info)
{
  K_Task *target = K_GetMainTask(info->Target);
  K_Task *current = K_GetCurrentTask();
  K_HANDLE mirror = NULL, source = info->Source;
  K_USIZE shift = ((K_USIZE)source & K_PAGE_FLAGS_MASK);
  K_USIZE offset, page, size = info->Size + shift;

  if (target && target->PageMap != current->PageMap)
  {
    K_SetPageMap(target->PageMap);
    mirror = K_FindLastFreeAddress(size);
    if (info) for (offset = 0; offset < size; offset += K_PAGE_SIZE)
    {
      K_SetPageMap(current->PageMap);
      page = K_GetPage(source + offset);
      K_SetPageMap(target->PageMap);
      (void)K_SetPage(mirror + offset, page | K_PAGE_EXTERNAL);
    }
    K_SetPageMap(current->PageMap);
  }
  if (!(info->Mirror = mirror))
  {
    if (target && target->PageMap != current->PageMap) mirror = source;
    info->Source = NULL;
    info->Target = 0;
    info->Size = 0;
  }
  else info->Mirror = mirror += shift;
  return mirror;
}

K_ShareInfo *K_CreateShareInfo()
{
  K_HANDLE page = K_FindLastFreeAddress(K_SHARE_INFO_SIZE);
  if (!page || !K_AllocatePages(page, K_SHARE_INFO_SIZE, K_PAGE_WRITABLE | K_PAGE_READABLE)) return NULL;
  return K_ZeroMemory(page, K_SHARE_INFO_SIZE);
}

void K_DeleteShareInfo(K_ShareInfo *info)
{
  K_USIZE index = K_MAX_SHARE_INFO_COUNT;
  if (info)
  {
    while (index--) if (info[index].Target)
    {
      K_ClearShareInfo(info + index);
    }
    K_FreePages(info, K_SHARE_INFO_SIZE);
  }
}

K_HANDLE K_ShareMemory(K_USIZE target, K_HANDLE address, K_USIZE size)
{
  K_Task *task = K_GetCurrentTask();
  K_USIZE index = K_MAX_SHARE_INFO_COUNT;
  K_HANDLE result = NULL;
  
  if (task && address)
  {
    if (size)
    {
      while (index-- && task->pShareInfo[index].Target);
      if (~index)
      {
        task->pShareInfo[index].Source = address;
        task->pShareInfo[index].Target = target;
        task->pShareInfo[index].Size = size;
        result = K_ApplyShareInfo(task->pShareInfo + index);
      }
    }
    else if (target) while (index--)
    {
      if (task->pShareInfo[index].Source == address && task->pShareInfo[index].Target == target)
      {
        K_ClearShareInfo(task->pShareInfo + index);
        result = (K_HANDLE)TRUE;
      }
    }
    else while (index--) if (task->pShareInfo[index].Source == address && task->pShareInfo[index].Target)
    {
      K_ClearShareInfo(task->pShareInfo + index);
      result = (K_HANDLE)TRUE;
    }
  }
  return result;
}

