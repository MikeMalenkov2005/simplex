#include "syscall.h"

#include <sys/syscall.h>
#include "task.h"

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

void K_SystemCallDispatch(K_USIZE index, K_USIZE arg1, K_USIZE arg2, K_USIZE arg3)
{
  (void)arg3;
  K_SetTaskR0(-1);
  switch (index)
  {
  case SYS_EXIT:
    (void)K_DeleteTask(K_GetCurrentTask());
    break;
  case SYS_SEND:
    if (K_CallSendMessage((void*)arg1, K_GetTask((K_U32)arg2))) K_SetTaskR0(0);
    break;
  case SYS_POLL:
    K_SetTaskR0(K_CallPollMessage((void*)arg1));
    break;
  case SYS_WAIT:
    (void)K_WaitMessage((void*)arg1);
    break;
  case SYS_PEEK:
    K_SetTaskR0(K_CallPeekMessage((void*)arg1));
    break;
  }
}

