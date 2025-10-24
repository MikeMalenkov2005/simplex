#include "message.h"

#include "memory.h"
#include "task.h"

K_MessageQueue *K_CreateMessageQueue()
{
  K_MessageQueue *queue = K_FindFirstFreeAddress(sizeof *queue);
  if (queue && K_AllocatePages(queue, sizeof *queue, K_PAGE_WRITABLE | K_PAGE_READABLE))
  {
    queue->Head = 0;
    queue->Tail = 0;
  }
  else queue = NULL;
  return queue;
}

void K_DeleteMessageQueue(K_MessageQueue *queue)
{
  if (queue) (void)K_FreePages(queue, sizeof *queue);
}

K_BOOL K_PushMessage(K_MessageQueue *queue, K_Message *message)
{
  if (queue->Head >= K_MESSAGE_LIMIT) return FALSE;
  queue->Buffer[queue->Head++] = *message;
  if (queue->Head >= K_MESSAGE_LIMIT) queue->Head = 0;
  if (queue->Head == queue->Tail) queue->Head = K_MESSAGE_LIMIT;
  return TRUE;
}

K_BOOL K_PollMessage(K_MessageQueue *queue, K_Message *message)
{
  K_U16 index = queue->Tail, next;
  if (queue->Head == queue->Tail) return FALSE;
  if (message->SenderID != K_TASK_INVALID_ID)
  {
    while (index != queue->Head)
    {
      if (queue->Buffer[index].SenderID == message->SenderID)
      {
        *message = queue->Buffer[index];
        while (index != queue->Tail)
        {
          next = (index ? index : K_MESSAGE_LIMIT) - 1;
          queue->Buffer[index] = queue->Buffer[next];
          index = next;
        }
        if (queue->Head >= K_MESSAGE_LIMIT) queue->Head = queue->Tail;
        if (++queue->Tail >= K_MESSAGE_LIMIT) queue->Tail = 0;
        return TRUE;
      }
      if (++index >= K_MESSAGE_LIMIT) index = 0;
      if (index == queue->Tail) break;
    }
    return FALSE;
  }
  if (queue->Head >= K_MESSAGE_LIMIT) queue->Head = queue->Tail;
  *message = queue->Buffer[queue->Tail++];
  if (queue->Tail >= K_MESSAGE_LIMIT) queue->Tail = 0;
  return TRUE;
}

K_BOOL K_PeekMessage(K_MessageQueue *queue, K_Message *message)
{
  K_U16 index = queue->Tail;
  if (queue->Head == queue->Tail) return FALSE;
  if (message->SenderID != K_TASK_INVALID_ID)
  {
    while (index != queue->Head)
    {
      if (queue->Buffer[index].SenderID == message->SenderID)
      {
        *message = queue->Buffer[index];
        return TRUE;
      }
      if (++index >= K_MESSAGE_LIMIT) index = 0;
      if (index == queue->Tail) break;
    }
    return FALSE;
  }
  *message = queue->Buffer[queue->Tail];
  return TRUE;
}

