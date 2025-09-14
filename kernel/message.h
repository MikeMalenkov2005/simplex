#ifndef _K_MESSAGE_H
#define _K_MESSAGE_H

#include <types.h>
#include <sys/limits.h>

struct K_MessagePayload
{
  K_U8 Data[K_MESSAGE_SIZE];
};

typedef struct K_MessagePayload K_MessagePayload;

struct K_Message
{
  K_U32 SenderID;
  K_MessagePayload Payload;
};

typedef struct K_Message K_Message;

struct K_MessageQueue
{
  K_U16 Head, Tail;
  K_Message Buffer[K_MESSAGE_LIMIT];
};

typedef struct K_MessageQueue K_MessageQueue;

K_MessageQueue *K_CreateMessageQueue();
void K_DeleteMessageQueue(K_MessageQueue *queue);

K_BOOL K_PushMessage(K_MessageQueue *queue, K_Message *message);
K_BOOL K_PollMessage(K_MessageQueue *queue, K_Message *message);
K_BOOL K_PeekMessage(K_MessageQueue *queue, K_Message *message);

#endif

