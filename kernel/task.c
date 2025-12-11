#include "task.h"
#include "share.h"
#include "timer.h"
#include "memory.h"
#include "utils.h"
#include "tls.h"

static K_U32 K_NextTaskID = 0;
static K_U32 K_NextGroupID = 0;
static K_USIZE K_CurrentSlot = K_TASK_LIMIT;
static K_Task *K_CurrentTaskIRQ = NULL;
static K_Task K_TaskSlots[K_TASK_LIMIT];

extern K_HANDLE K_CreateContext(K_USIZE stack, K_U16 flags);
extern void K_DeleteContext(K_HANDLE context);
extern void K_SaveContext(K_HANDLE context);
extern void K_LoadContext(K_HANDLE context);

static void K_ClearTaskSlot(K_Task *task)
{
  task->pTLS = NULL;
  task->pShareInfo = NULL;
  task->WaitInfo = NULL;
  task->PageMap = NULL;
  task->Context = NULL;
  task->Handler = NULL;
  task->TargetID = K_TASK_INVALID_ID;
  task->ParentID = K_TASK_INVALID_ID;
  task->TaskID = K_TASK_INVALID_ID;
  task->GroupID = K_TASK_INVALID_ID;
  task->ClusterID = K_TASK_INVALID_ID;
  task->Flags = 0;
  task->Mode = 0;
}

void K_InitTaskSlots(void)
{
  K_U32 i = K_TASK_LIMIT;
  while (i--) K_ClearTaskSlot(K_TaskSlots + i);
}

K_Task *K_GetCurrentTask(void)
{
  return K_CurrentTaskIRQ ? K_CurrentTaskIRQ
    : (K_CurrentSlot < K_TASK_LIMIT ? K_TaskSlots + K_CurrentSlot : NULL);
}

K_BOOL K_SetCurrentTask(K_Task *task)
{
  K_USIZE slot = (K_USIZE)(task - K_TaskSlots);
  K_Task *current = K_TaskSlots + K_CurrentSlot;
  if (!K_CurrentTaskIRQ && current && task && slot < K_TASK_LIMIT)
  {
    if (current != task)
    {
      K_SaveContext(current->Context);
      if (current->PageMap != task->PageMap) K_SetPageMap(task->PageMap);
      K_LoadContext(task->Context);
      K_CurrentSlot = slot;
    }
    return TRUE;
  }
  return FALSE;
}

K_Task *K_GetTask(K_U32 tid)
{
  K_U32 i = K_TASK_LIMIT;
  while (i--) if (K_TaskSlots[i].TaskID == tid) return K_TaskSlots + i;
  return NULL;
}

K_Task *K_GetMainTask(K_U32 gid)
{
  K_U32 i = K_TASK_LIMIT;
  while (i--) if (K_TaskSlots[i].GroupID == gid &&
      !(K_TaskSlots[i].Flags & K_TASK_THREAD)) return K_TaskSlots + i;
  return NULL;
}

K_Task *K_CreateTask(K_USIZE stack, K_U16 flags)
{
  K_HANDLE map = K_GetPageMap();
  K_Task *task = NULL;
  K_U32 i = K_TASK_LIMIT;
  if (K_CurrentTaskIRQ || K_NextTaskID == K_TASK_INVALID_ID) return NULL;
  if (K_NextGroupID > K_TASK_MAX_GROUP_ID &&
      (K_CurrentSlot >= K_TASK_LIMIT || (flags & K_TASK_THREAD))) return NULL;
  while (i--) if (!K_TaskSlots[i].Mode)
  {
    task = K_TaskSlots + i;
    break;
  }
  if (task)
  {
    if (K_CurrentSlot < K_TASK_LIMIT)
    {
      if (flags & K_TASK_THREAD)
        task->PageMap = K_TaskSlots[K_CurrentSlot].PageMap;
      else if (!(task->PageMap = K_CreatePageMap())) return NULL;
      task->ParentID = K_TaskSlots[K_CurrentSlot].TaskID;
    }
    else
    {
      task->PageMap = map;
      flags &= ~K_TASK_THREAD;
      K_CurrentSlot = i;
    }

    if (map != task->PageMap) K_SetPageMap(task->PageMap);
    if (!(task->pTLS = K_CreateTLS((flags & K_TASK_THREAD)
            ? K_TaskSlots[K_CurrentSlot].pTLS : NULL)) ||
        !(task->pShareInfo = K_CreateShareInfo()) ||
        !(task->Context = K_CreateContext(stack, flags))) 
    {
      if (map != task->PageMap) K_SetPageMap(map);
      K_DeleteTask(task);
      return NULL;
    }
    if (map != task->PageMap) K_SetPageMap(map);
    
    if (K_CurrentSlot == i) K_LoadContext(task->Context);

    task->GroupID = (flags & K_TASK_THREAD)
      ? K_TaskSlots[K_CurrentSlot].GroupID : K_NextGroupID++;
    task->TaskID = K_NextTaskID++;
    task->Flags = flags;
    task->Mode = K_TASK_MODE_RUNNING;
  }
  return task;
}

K_BOOL K_DeleteTask(K_Task *task)
{
  K_HANDLE map;
  K_U32 i = K_TASK_LIMIT;
  if (!task || K_CurrentTaskIRQ) return FALSE;
  if (!(task->Flags & K_TASK_THREAD)) while (i--)
  {
    if (K_TaskSlots[i].GroupID == task->GroupID &&
        (K_TaskSlots[i].Flags & K_TASK_THREAD))
    {
      (void)K_DeleteTask(K_TaskSlots + i);
    }
  }
  if (task == K_TaskSlots + K_CurrentSlot && !K_SwitchTask()) return FALSE;
  map = K_GetPageMap();
  if (task->Flags & K_TASK_THREAD)
  {
    if (map != task->PageMap) K_SetPageMap(task->PageMap);
    K_DeleteTLS(task->pTLS);
    K_DeleteShareInfo(task->pShareInfo);
    K_DeleteContext(task->Context);
    if (map != task->PageMap) K_SetPageMap(map);
  }
  else if (!K_DeletePageMap(task->PageMap)) return FALSE;
  K_ClearTaskSlot(task);
  return TRUE;
}

static K_BOOL K_TransferMessage(K_Task *source, K_Task *target)
{
  K_U8 buffer[K_MESSAGE_SIZE];
  if (target->Mode != K_TASK_MODE_WAIT_MESSAGE) return FALSE;
  if (target->TargetID != source->TaskID &&
      target->TargetID != K_TASK_INVALID_ID) return FALSE;
  if (!K_SetCurrentTask(source)) return FALSE;
  if (source->PageMap != target->PageMap)
    memcpy(buffer, source->WaitInfo, sizeof(buffer));
  if (!K_SetCurrentTask(target)) return FALSE;
  if (source->PageMap != target->PageMap)
    memcpy(target->WaitInfo, buffer, sizeof(buffer));
  else memcpy(target->WaitInfo, source->WaitInfo, sizeof(buffer));
  target->WaitInfo = NULL;
  target->TargetID = K_TASK_INVALID_ID;
  target->Mode = K_TASK_MODE_RUNNING;
  K_SetTaskR0(target, source->TaskID);
  return TRUE;
}

K_BOOL K_SwitchTask(void)
{
  K_Task *target, *task;
  K_USIZE slot = K_CurrentSlot;

  if (!K_CurrentTaskIRQ)
  {
    while ((slot = (slot + 1) % K_TASK_LIMIT) != K_CurrentSlot)
    {
      task = K_TaskSlots + slot;
      switch (task->Mode)
      {
      case K_TASK_MODE_RUNNING:
        if (K_SetCurrentTask(task)) return TRUE;
        break;
      case K_TASK_MODE_SEND_MESSAGE:
        if (!(target = K_GetTask(task->TargetID)))
        {
          task->WaitInfo = NULL;
          task->TargetID = K_TASK_INVALID_ID;
          task->Mode = K_TASK_MODE_RUNNING;
          if (K_SetCurrentTask(task))
          {
            K_SetTaskR0(task, -1);
            return TRUE;
          }
          else K_SetTaskR0(task, -1);
        }
        else if (K_TransferMessage(task, target))
        {
          task->Mode = K_TASK_MODE_WAIT_MESSAGE;
          return TRUE;
        }
        break;
      case K_TASK_MODE_WAIT_MESSAGE:
        if (task->TargetID != K_TASK_INVALID_ID && !K_GetTask(task->TargetID))
        {
          task->WaitInfo = NULL;
          task->TargetID = K_TASK_INVALID_ID;
          task->Mode = K_TASK_MODE_RUNNING;
          if (K_SetCurrentTask(task))
          {

            K_SetTaskR0(task, -1);
            return TRUE;
          }
          else K_SetTaskR0(task, -1);
        }
        break;
      case K_TASK_MODE_WAIT_TIME:
        if ((K_SSIZE)((K_USIZE)task->WaitInfo - K_Ticks) <= 0)
        {
          task->WaitInfo = NULL;
          task->Mode = K_TASK_MODE_RUNNING;
          if (K_SetCurrentTask(task)) return TRUE;
        }
        break;
      }
    }
  }
  return FALSE;
}

K_BOOL K_WaitTicks(K_USIZE duration)
{
  K_Task *task = K_GetCurrentTask();
  if (!task || !K_SwitchTask()) return FALSE;
  task->WaitInfo = (K_HANDLE)(K_Ticks + duration);
  task->Mode = K_TASK_MODE_WAIT_TIME;
  return TRUE;
}

K_BOOL K_SendMessage(K_HANDLE buffer, K_Task *target)
{
  K_Task *task = K_GetCurrentTask();
  if (!task || !target || task == target || K_CurrentTaskIRQ ||
      !K_IsUserRange(buffer, K_MESSAGE_SIZE,
        K_PAGE_READABLE | K_PAGE_WRITABLE)) return FALSE;
  task->WaitInfo = buffer;
  task->TargetID = target->TaskID;
  task->Mode = K_TASK_MODE_SEND_MESSAGE;
  if (K_TransferMessage(task, target)) task->Mode = K_TASK_MODE_WAIT_MESSAGE;
  else if (!K_SwitchTask())
  {
    task->WaitInfo = NULL;
    task->TargetID = K_TASK_INVALID_ID;
    task->Mode = K_TASK_MODE_RUNNING;
    return FALSE;
  }
  return TRUE;
}

K_BOOL K_PollMessage(K_HANDLE buffer)
{
  K_Task *target, *task = K_GetCurrentTask();
  K_USIZE i = K_TASK_LIMIT;
  if (!task || K_CurrentTaskIRQ || !K_IsUserRange(buffer,
        K_MESSAGE_SIZE, K_PAGE_READABLE | K_PAGE_WRITABLE)) return FALSE;
  target = K_GetTask(task->TargetID);
  task->WaitInfo = buffer;
  task->TargetID = K_TASK_INVALID_ID;
  task->Mode = K_TASK_MODE_WAIT_MESSAGE;
  if (target && target != task) (void)K_TransferMessage(task, target);
  while (i--) if (K_TaskSlots[i].TargetID == task->TaskID &&
      K_TaskSlots[i].Mode == K_TASK_MODE_SEND_MESSAGE &&
      K_TransferMessage(K_TaskSlots + i, task))
  {
    K_TaskSlots[i].Mode = K_TASK_MODE_WAIT_MESSAGE;
    return TRUE;
  }
  task->WaitInfo = NULL;
  task->Mode = K_TASK_MODE_RUNNING;
  return FALSE;
}

K_BOOL K_WaitMessage(K_HANDLE buffer)
{
  K_Task *task = K_GetCurrentTask();
  K_USIZE i = K_TASK_LIMIT;
  if (!task || K_CurrentTaskIRQ || !K_IsUserRange(buffer,
        K_MESSAGE_SIZE, K_PAGE_READABLE | K_PAGE_WRITABLE)) return FALSE;
  task->WaitInfo = buffer;
  task->TargetID = K_TASK_INVALID_ID;
  task->Mode = K_TASK_MODE_WAIT_MESSAGE;
  while (i--) if (K_TaskSlots[i].TargetID == task->TaskID &&
      K_TaskSlots[i].Mode == K_TASK_MODE_SEND_MESSAGE &&
      K_TransferMessage(K_TaskSlots + i, task))
  {
    K_TaskSlots[i].Mode = K_TASK_MODE_WAIT_MESSAGE;
    return TRUE;
  }
  if (!K_SwitchTask())
  {
    task->WaitInfo = NULL;
    task->TargetID = K_TASK_INVALID_ID;
    task->Mode = K_TASK_MODE_RUNNING;
    return FALSE;
  }
  return TRUE;
}

extern void K_DebugHex(K_U16 x, K_U16 y, K_USIZE value);

K_BOOL K_FireMessage(K_HANDLE buffer, K_Task *target)
{
  K_BOOL result;
  K_Task *task = K_GetCurrentTask();
  if (!task || !target || task == target || K_CurrentTaskIRQ ||
      !K_IsUserRange(buffer, K_MESSAGE_SIZE,
        K_PAGE_READABLE | K_PAGE_WRITABLE)) return FALSE;
  task->WaitInfo = buffer;
  result = K_TransferMessage(task, target);
  task->WaitInfo = NULL;
  return result;
}

K_BOOL K_WaitTaskIRQ(K_USIZE irq)
{
  K_Task *task = K_GetCurrentTask();
  if (!task || !(task->Flags & K_TASK_MODULE)) return FALSE;
  K_EndTaskIRQ();
  if (task == K_TaskSlots + K_CurrentSlot && !K_SwitchTask()) return FALSE;
  task->WaitInfo = (K_HANDLE)irq;
  task->Mode = K_TASK_MODE_WAIT_IRQ;
  return TRUE;
}

K_BOOL K_BeginTaskIRQ(K_USIZE irq)
{
  K_USIZE i = K_TASK_LIMIT;
  while (i--) if ((K_USIZE)K_TaskSlots[i].WaitInfo == irq)
  {
    if (K_TaskSlots[i].Mode == K_TASK_MODE_WAIT_IRQ)
    {
      if (!K_CurrentTaskIRQ)
      {
        K_CurrentTaskIRQ = K_TaskSlots + i;
        K_SaveContext(K_TaskSlots[K_CurrentSlot].Context);
        K_SetPageMap(K_CurrentTaskIRQ->PageMap);
        K_LoadContext(K_CurrentTaskIRQ->Context);
      }
      K_TaskSlots[i].Mode = K_TASK_MODE_PROCESS_IRQ;
      return TRUE;
    }
    if (K_TaskSlots[i].Mode == K_TASK_MODE_PROCESS_IRQ) return TRUE;
  }
  return FALSE;
}

void K_EndTaskIRQ(void)
{
  K_Task *task;
  K_USIZE i = K_TASK_LIMIT;
  if (K_CurrentTaskIRQ)
  {
    K_CurrentTaskIRQ->WaitInfo = 0;
    K_CurrentTaskIRQ->Mode = K_TASK_MODE_RUNNING;
    K_SaveContext(K_CurrentTaskIRQ->Context);
    K_CurrentTaskIRQ = NULL;
    while (!K_CurrentTaskIRQ && i--)
      if (K_TaskSlots[i].Mode == K_TASK_MODE_PROCESS_IRQ)
        K_CurrentTaskIRQ = K_TaskSlots + i;
    task = K_GetCurrentTask();
    K_SetPageMap(task->PageMap);
    K_LoadContext(task->Context);
  }
}

