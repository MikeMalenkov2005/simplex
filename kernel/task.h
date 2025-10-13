#ifndef _K_TASK_H
#define _K_TASK_H

#include "message.h"
#include "share.h"
#include "tls.h"

#define K_TASK_LIMIT  1024

#define K_TASK_INVALID_ID (~(K_U32)0)
#define K_TASK_MAX_GROUP_ID (K_TASK_INVALID_ID >> 1)

#define K_TASK_MODULE (1 << 0)
#define K_TASK_THREAD (1 << 1)

#define K_TASK_MODE_RUNNING       1
#define K_TASK_MODE_WAIT_MESSAGE  2
#define K_TASK_MODE_WAIT_TIME     3
#define K_TASK_MODE_WAIT_IRQ      4
#define K_TASK_MODE_PROCESS_IRQ   5

struct K_Task
{
  K_TLS *pTLS;
  K_ShareInfo *pShareInfo;
  K_MessageQueue *pMessageQueue;
  K_HANDLE WaitInfo;
  K_HANDLE PageMap;
  K_HANDLE Context;
  K_HANDLE Handler;
  K_U32 ParentID;   /* POSIX: parent thread id  */
  K_U32 TaskID;     /* POSIX: thread id         */
  K_U32 GroupID;    /* POSIX: process id        */
  K_U32 ClusterID;  /* POSIX: process group id  */
  K_U16 Flags;
  K_U16 Mode;
};

typedef struct K_Task K_Task;

void K_InitTaskSlots();
K_Task *K_GetCurrentTask();

K_Task *K_GetTask(K_U32 tid);
K_Task *K_GetMainTask(K_U32 gid);

K_Task *K_CreateTask(K_USIZE stack, K_U16 flags);
K_BOOL K_DeleteTask(K_Task *task);
K_BOOL K_SwitchTask();

K_BOOL K_WaitTicks(K_U32 duration);

K_BOOL K_SendMessage(K_Task *target, K_Message *message);
K_BOOL K_WaitMessage(K_MessagePayload *buffer);

K_BOOL K_WaitTaskIRQ(K_USIZE irq);
K_BOOL K_BeginTaskIRQ(K_USIZE irq);
void K_EndTaskIRQ();

K_HANDLE K_GetTaskIP(K_Task *task);
void K_SetTaskIP(K_Task *task, K_HANDLE ip);

K_SSIZE K_GetTaskR0(K_Task *task);
void K_SetTaskR0(K_Task *task, K_SSIZE r0);

K_BOOL K_TaskPush(K_Task *task, K_USIZE value);
K_BOOL K_TaskPop(K_Task *task, K_USIZE *value);

#endif

