#include "syscall.h"
#include "share.h"

#define __CRT_MAP_FLAGS

#include <sys/syscall.h>
#include <sys/memory.h>
#include "memory.h"
#include "task.h"
#include "tls.h"

K_BOOL K_CallSendMessage(K_MessagePayload *payload, K_Task *target)
{
  K_Message message;
  message.SenderID = K_GetCurrentTask()->TaskID;
  message.Payload = *payload;
  return K_SendMessage(target, &message);
}

K_SSIZE K_CallPollMessage(K_MessagePayload *buffer)
{
  K_Message message;
  if (!K_PollMessage(K_GetCurrentTask()->pMessageQueue, &message)) return -1;
  *buffer = message.Payload;
  return message.SenderID;
}

K_SSIZE K_CallPeekMessage(K_MessagePayload *buffer)
{
  K_Message message;
  if (!K_PeekMessage(K_GetCurrentTask()->pMessageQueue, &message)) return -1;
  *buffer = message.Payload;
  return message.SenderID;
}

K_SSIZE K_CallCreateThread(K_HANDLE entry, K_USIZE stack)
{
  K_Task *task = K_CreateTask(stack, K_GetCurrentTask()->Flags | K_TASK_THREAD);
  if (!task) return -1;
  K_SetTaskIP(task, entry);
  return task->TaskID;
}

K_BOOL K_CallSignalTask(K_Task *task, K_USIZE value)
{
  if (!task || !task->Handler || !K_TaskPush(task, value)) return FALSE;
  if (!K_TaskPush(task, (K_USIZE)K_GetTaskIP(task)))
  {
(void)K_TaskPop(task, &value);
    return FALSE;
  }
  K_SetTaskIP(task, task->Handler);
  return TRUE;
}

K_HANDLE K_CallMapMemory(K_USIZE size, K_USIZE mode)
{
  K_HANDLE result = (mode & MAP_UP)
    ? K_FindFirstFreeAddress(size)
    : K_FindLastFreeAddress(size);
  K_U16 flags = K_PAGE_USER_MODE;
  if (mode & MAP_RD) flags |= K_PAGE_READABLE;
  if (mode & MAP_WR) flags |= K_PAGE_WRITABLE;
  if (mode & MAP_EX) flags |= K_PAGE_EXECUTABLE;
  if (result && !K_AllocatePages(result, size, flags)) result = NULL;
  return result;
}

K_HANDLE K_CallMapDevice(K_USIZE start, K_USIZE size, K_USIZE mode)
{
  K_USIZE offset;
  K_HANDLE result = K_FindFirstFreeAddress(size);
  K_U16 flags = K_PAGE_USER_MODE | K_PAGE_EXTERNAL | K_PAGE_CACHE_DISABLE | K_PAGE_VALID;

  if (mode & MAP_RD) flags |= K_PAGE_READABLE;
  if (mode & MAP_WR) flags |= K_PAGE_WRITABLE;
  if (mode & MAP_EX) flags |= K_PAGE_EXECUTABLE;
  
  size = K_PageUp(size);
  if (result) for (offset = 0; offset < size; offset += K_PAGE_SIZE)
  {
    if (!K_SetPage(result + offset, (start + offset) | flags))
    {
      size = offset;
      for (offset = 0; offset < size; offset += K_PAGE_SIZE)
      {
        (void)K_SetPage(result + offset, 0);
      }
      result = NULL;
    }
  }
  return result;
}

K_BOOL K_CallFreeMapping(K_HANDLE address, K_USIZE size)
{
  K_USIZE offset;
  K_U16 flags = K_GetRangeFlags(address, size);
  if (((K_USIZE)address & K_PAGE_FLAGS_MASK) || !size || !(flags & K_PAGE_USER_MODE)) return FALSE;
  if (flags & K_PAGE_EXTERNAL)
  {
    size = K_PageUp(size);
    for (offset = 0; offset < size; ++offset)
    {
      (void)K_SetPage(address + offset, 0);
    }
    return TRUE;
  }
  return K_FreePages(address, size);
}

K_BOOL K_CallChangeMapping(K_HANDLE address, K_USIZE size, K_USIZE mode)
{
  K_U16 flags = K_GetRangeFlags(address, size);
  if (((K_USIZE)address & K_PAGE_FLAGS_MASK) || !size || !(flags & K_PAGE_USER_MODE)) return FALSE;
  flags = (mode & MAP_RD) ? (flags | K_PAGE_READABLE) : (flags & ~K_PAGE_READABLE);
  flags = (mode & MAP_WR) ? (flags | K_PAGE_WRITABLE) : (flags & ~K_PAGE_WRITABLE);
  flags = (mode & MAP_EX) ? (flags | K_PAGE_EXECUTABLE) : (flags & ~K_PAGE_EXECUTABLE);
  return K_ChangePages(address, size, flags);
}

void K_SystemCallDispatch(K_USIZE index, K_USIZE arg1, K_USIZE arg2, K_USIZE arg3)
{
  K_Task *task = K_GetCurrentTask();
  K_SetTaskR0(task, -1);
  switch (index)
  {
  case SYS_EXIT:
    (void)K_DeleteTask(task);
    break;
  case SYS_SEND:
    if (K_IsUserRange((K_HANDLE)arg1, K_MESSAGE_SIZE, K_PAGE_READABLE))
    {
      if (K_CallSendMessage((void*)arg1, K_GetTask((K_U32)arg2))) K_SetTaskR0(task, 0);
    }
    break;
  case SYS_POLL:
    if (K_IsUserRange((K_HANDLE)arg1, K_MESSAGE_SIZE, K_PAGE_WRITABLE))
    {
      K_SetTaskR0(task, K_CallPollMessage((void*)arg1));
    }
    break;
  case SYS_WAIT:
    if (K_IsUserRange((K_HANDLE)arg1, K_MESSAGE_SIZE, K_PAGE_WRITABLE))
    {
      (void)K_WaitMessage((void*)arg1);
    }
    break;
  case SYS_PEEK:
    if (K_IsUserRange((K_HANDLE)arg1, K_MESSAGE_SIZE, K_PAGE_WRITABLE))
    {
      K_SetTaskR0(task, K_CallPeekMessage((void*)arg1));
    }
    break;
  case SYS_GET_TASK_ID:
    K_SetTaskR0(task, (K_SSIZE)task->TaskID);
    break;
  case SYS_GET_GROUP_ID:
    K_SetTaskR0(task, (K_SSIZE)task->GroupID);
    break;
  case SYS_GET_PARENT_ID:
    K_SetTaskR0(task, (K_SSIZE)task->ParentID);
    break;
  case SYS_GET_LEADER_ID:
    K_SetTaskR0(task, (K_SSIZE)K_GetMainTask(task->GroupID));
    break;
  case SYS_CREATE_THREAD:
    K_SetTaskR0(task, K_CallCreateThread((K_HANDLE)arg1, arg2));
    break;
  case SYS_SWITCH_TASK:
    K_SetTaskR0(task, 0);
    if (!K_SwitchTask()) K_SetTaskR0(task, -1);
    break;
  case SYS_SIGNAL_TASK:
    if (K_CallSignalTask(K_GetTask((K_U32)arg1), arg2)) K_SetTaskR0(task, 0);
    break;
  case SYS_SET_HANDLER:
    K_SetTaskR0(task, (K_USIZE)task->Handler);
    task->Handler = (K_HANDLE)arg1;
    break;
  case SYS_TLS_NEW:
    K_SetTaskR0(task, K_TLSNewEntry(task->pTLS));
    break;
  case SYS_TLS_GET:
    K_SetTaskR0(task, K_TLSGetEntry(task->pTLS, arg1));
    break;
  case SYS_TLS_SET:
    if (K_TLSSetEntry(task->pTLS, arg1, arg2)) K_SetTaskR0(task, 0);
    break;
  case SYS_GROUP_EXIT:
    (void)K_DeleteTask(K_GetMainTask(task->GroupID));
    break;
  case SYS_MAP:
    if (!arg1)
    {
      K_SetTaskR0(task, (K_SSIZE)(K_USIZE)K_CallMapMemory(arg2, arg3));
    }
    else if (!(arg1 & K_PAGE_FLAGS_MASK) && (task->Flags & K_TASK_MODULE))
    {
      K_SetTaskR0(task, (K_SSIZE)(K_USIZE)K_CallMapDevice(arg1, arg2, arg3));
    }
    else K_SetTaskR0(task, 0);
    break;
  case SYS_FREE:
    if (K_CallFreeMapping((K_HANDLE)arg1, arg2)) K_SetTaskR0(task, 0);
    break;
  case SYS_REMAP:
    if (K_CallChangeMapping((K_HANDLE)arg1, arg2, arg3)) K_SetTaskR0(task, 0);
    break;
  case SYS_SHARE:
    /* TODO: Test the implementation!!! */
    K_SetTaskR0(task, (K_USIZE)K_ShareMemory(arg3, (K_HANDLE)arg1, arg2) ^ !!arg2);
    break;
  case SYS_IRQ_WAIT:
    K_SetTaskR0(task, 0);
    if (!K_WaitTaskIRQ(arg1)) K_SetTaskR0(task, -1);
    break;
  case SYS_IRQ_EXIT:
    K_SetTaskR0(task, 0);
    K_EndTaskIRQ();
    break;
  }
}

