#include "message.h"

#include "memory.h"

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
  if (queue->Head >= K_MESSAGE_LIMIT) return queue->Head = 0;
  if (queue->Head == queue->Tail) queue->Head = K_MESSAGE_LIMIT;
  return TRUE;
}

K_BOOL K_PollMessage(K_MessageQueue *queue, K_Message *message)
{
  if (queue->Head == queue->Tail) return FALSE;
  if (queue->Head >= K_MESSAGE_LIMIT) queue->Head = queue->Tail;
  *message = queue->Buffer[queue->Tail++];
  if (queue->Tail >= K_MESSAGE_LIMIT) queue->Tail = 0;
  return TRUE;
}

K_BOOL K_PeekMessage(K_MessageQueue *queue, K_Message *message)
{
  if (queue->Head == queue->Tail) return FALSE;
  *message = queue->Buffer[queue->Tail];
  return TRUE;
}

