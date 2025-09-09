#include "task.h"
#include "timer.h"
#include "memory.h"

static K_U32 K_NextTaskID = 0;
static K_U32 K_NextGroupID = 0;
static K_USIZE K_CurrentSlot = K_TASK_LIMIT;
static K_Task *K_CurrentTaskIRQ = NULL;
static K_Task K_TaskSlots[K_TASK_LIMIT];

extern K_HANDLE K_CreateContext(K_USIZE stack, K_U16 flags);
extern void K_DeleteContext(K_HANDLE context);
extern void K_SaveContext(K_HANDLE context);
extern void K_LoadContext(K_HANDLE context);

void K_ClearTaskSlot(K_Task *task)
{
  task->pMessageQueue = NULL;
  task->WaitInfo = NULL;
  task->PageMap = NULL;
  task->Context = NULL;
  task->ParentID = K_TASK_INVALID_ID;
  task->GroupID = K_TASK_INVALID_ID;
  task->TaskID = K_TASK_INVALID_ID;
  task->Flags = 0;
  task->Mode = 0;
}

void K_InitTaskSlots()
{
  K_U32 i = K_TASK_LIMIT;
  while (i--) K_ClearTaskSlot(K_TaskSlots + i);
}

K_Task *K_GetCurrentTask()
{
  return K_CurrentTaskIRQ ? K_CurrentTaskIRQ : (K_CurrentSlot < K_TASK_LIMIT ? K_TaskSlots + K_CurrentSlot : NULL);
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
  while (i--) if (K_TaskSlots[i].GroupID == gid && !(K_TaskSlots[i].Flags & K_TASK_THREAD)) return K_TaskSlots + i;
  return NULL;
}

K_Task *K_CreateTask(K_USIZE stack, K_U16 flags)
{
  K_HANDLE map = K_GetPageMap();
  K_Task *task = NULL;
  K_U32 i = K_TASK_LIMIT;
  if (K_CurrentTaskIRQ || K_NextTaskID == K_TASK_INVALID_ID) return NULL;
  while (i--) if (!K_TaskSlots[i].Mode)
  {
    task = K_TaskSlots + i;
    break;
  }
  if (task)
  {
    if (K_CurrentSlot < K_TASK_LIMIT)
    {
      if (flags & K_TASK_THREAD) task->PageMap = K_TaskSlots[K_CurrentSlot].PageMap;
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
    if (!(task->pMessageQueue = K_CreateMessageQueue()) || !(task->Context = K_CreateContext(stack, flags))) 
    {
      if (map != task->PageMap) K_SetPageMap(map);
      K_DeleteTask(task);
      return NULL;
    }
    if (map != task->PageMap) K_SetPageMap(map);
    
    if (K_CurrentSlot == i) K_LoadContext(task->Context);

    task->GroupID = (flags & K_TASK_THREAD) ? K_TaskSlots[K_CurrentSlot].GroupID : K_NextGroupID++;
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
    if (K_TaskSlots[i].GroupID == task->GroupID && (K_TaskSlots[i].Flags & K_TASK_THREAD))
    {
      (void)K_DeleteTask(K_TaskSlots + i);
    }
  }
  if (task == K_TaskSlots + K_CurrentSlot && !K_SwitchTask()) return FALSE;
  map = K_GetPageMap();
  if (task->Flags & K_TASK_THREAD)
  {
    if (map != task->PageMap) K_SetPageMap(task->PageMap);
    K_DeleteMessageQueue(task->pMessageQueue);
    K_DeleteContext(task->Context);
    if (map != task->PageMap) K_SetPageMap(map);
  }
  else if (!K_DeletePageMap(task->PageMap)) return FALSE;
  K_ClearTaskSlot(task);
  return TRUE;
}

K_BOOL K_SwitchTask()
{
  K_Message message;
  K_U32 limit = K_TASK_LIMIT;
  K_BOOL switched = FALSE;
  message.SenderID = K_TASK_INVALID_ID;

  if (!K_CurrentTaskIRQ)
  {
    K_SaveContext(K_TaskSlots[K_CurrentSlot].Context);
    while (!switched && limit--)
    {
      K_CurrentSlot = (K_CurrentSlot + 1) % K_TASK_LIMIT;
      if (K_TaskSlots[K_CurrentSlot].Mode) K_SetPageMap(K_TaskSlots[K_CurrentSlot].PageMap);
      switch (K_TaskSlots[K_CurrentSlot].Mode)
      {
      case K_TASK_MODE_WAIT_MESSAGE:
        if (K_PollMessage(K_TaskSlots[K_CurrentSlot].pMessageQueue, &message))
        {
          *(K_MessagePayload*)K_TaskSlots[K_CurrentSlot].WaitInfo = message.Payload;
          K_TaskSlots[K_CurrentSlot].Mode = K_TASK_MODE_RUNNING;
          K_TaskSlots[K_CurrentSlot].WaitInfo = NULL;
        }
        break;
      case K_TASK_MODE_WAIT_TIME:
        if ((K_S32)(K_U32)((K_USIZE)K_TaskSlots[K_CurrentSlot].WaitInfo - K_Ticks) <= 0)
        {
          K_TaskSlots[K_CurrentSlot].Mode = K_TASK_MODE_RUNNING;
          K_TaskSlots[K_CurrentSlot].WaitInfo = NULL;
        }
        break;
      }
      switched = K_TaskSlots[K_CurrentSlot].Mode == K_TASK_MODE_RUNNING;
    }
    if (limit >= K_TASK_LIMIT) switched = FALSE;
    else K_LoadContext(K_TaskSlots[K_CurrentSlot].Context);
    if (message.SenderID != K_TASK_INVALID_ID) K_SetTaskR0(K_TaskSlots + K_CurrentSlot, (K_SSIZE)message.SenderID);
  }
  return switched;
}

K_BOOL K_WaitTicks(K_U32 duration)
{
  K_Task *task = K_GetCurrentTask();
  if (!task || !K_SwitchTask()) return FALSE;
  task->WaitInfo = (K_HANDLE)(K_USIZE)(K_U32)(K_Ticks + duration);
  task->Mode = K_TASK_MODE_WAIT_TIME;
  return TRUE;
}

K_BOOL K_SendMessage(K_Task *target, K_Message *message)
{
  K_HANDLE map;
  K_BOOL result = FALSE;
  if (target)
  {
    map = K_GetPageMap();
    if (map != target->PageMap) K_SetPageMap(target->PageMap);
    result = K_PushMessage(target->pMessageQueue, message);
    if (map != target->PageMap) K_SetPageMap(map);
  }
  return result;
}

K_BOOL K_WaitMessage(K_MessagePayload *buffer)
{
  K_Task *task = K_GetCurrentTask();
  K_Message message;
  if (!task) return FALSE;
  if (!K_PollMessage(task->pMessageQueue, &message))
  {
    if (!K_SwitchTask()) return FALSE;
    task->WaitInfo = buffer;
    task->Mode = K_TASK_MODE_WAIT_MESSAGE;
  }
  else
  {
    K_SetTaskR0(task, (K_SSIZE)message.SenderID);
    *buffer = message.Payload;
  }
  return TRUE;
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

void K_EndTaskIRQ()
{
  K_Task *task;
  K_USIZE i = K_TASK_LIMIT;
  if (K_CurrentTaskIRQ)
  {
    K_CurrentTaskIRQ->WaitInfo = 0;
    K_CurrentTaskIRQ->Mode = K_TASK_MODE_RUNNING;
    K_SaveContext(K_CurrentTaskIRQ->Context);
    K_CurrentTaskIRQ = NULL;
    while (!K_CurrentTaskIRQ && i--) if (K_TaskSlots[i].Mode == K_TASK_MODE_PROCESS_IRQ)
    {
      K_CurrentTaskIRQ = K_TaskSlots + i;
    }
    task = K_GetCurrentTask();
    K_SetPageMap(task->PageMap);
    K_LoadContext(task->Context);
  }
}

