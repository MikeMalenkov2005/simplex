#include "ps2.h"

#include <drv/x86ps2.h>
#include <sys/limits.h>
#include <simplex.h>
#include <x86/pio.h>
#include <string.h>

static PS2_Buffer input;
static PS2_Queue queue;

static volatile PS2_Command command;
static volatile int ready;

K_BOOL PS2_BufferPull(PS2_Buffer *buffer, K_U8 *byte)
{
  if (!buffer || buffer->head == buffer->tail) return FALSE;
  if (byte) *byte = buffer->data[buffer->tail];
  if (buffer->head < 0) buffer->head = buffer->tail;
  buffer->tail = (buffer->tail + 1) & BUFFER_MASK;
  return TRUE;
}

K_BOOL PS2_BufferPush(PS2_Buffer *buffer, K_U8 byte)
{
  if (!buffer) return FALSE;
  if (buffer->head < 0)
  {
    buffer->data[buffer->tail++] = byte;
    buffer->tail &= BUFFER_MASK;
  }
  else
  {
    buffer->data[buffer->head++] = byte;
    if ((buffer->head &= BUFFER_MASK) == buffer->tail) buffer->head = -1;
  }
  return TRUE;
}

void irq_handler()
{
  K_U8 message[K_MESSAGE_SIZE];
  int i, ltid = sys_getltid();
  K_U8 byte, left = 0;

  while (sys_irq_wait(1) != -1)
  {
    i = 1;
    while (PIO_Read8(0x64) & 1)
    {
      byte = PIO_Read8(0x60);
      if (left)
      {
        command.data[command.returns - --left] = byte;
        if (!left) ready = command.returns + 1;
      }
      else if (!ready && command.sender != -1 && byte == 0xFA)
      {
        command.data[0] = byte;
        ready = !(left = command.returns);
      }
      else if (!ready && command.sender != -1 && (byte == 0xFE || byte == 0xEE))
      {
        command.data[0] = byte;
        ready = 1;
      }
      else
      {
        message[i++] = byte;
        if (i == K_MESSAGE_SIZE)
        {
          message[0] = i;
          sys_send(message, ltid);
          i = 1;
        }
      }
    }
    if ((message[0] = i) > 1) sys_send(message, ltid);
  }
}

int main()
{
  int irq1, tid, i;
  K_U8 message[K_MESSAGE_SIZE];
  command.sender = -1;
  for (i = 0; i < QUEUE_SIZE; ++i) queue.commands[i].sender = -1;
  if ((irq1 = sys_thread(irq_handler, 0)) == -1) return 1;
  while ((tid = sys_poll(message, -1)))
  {
    if (tid == irq1)
    {
      for (i = 1; i < message[0] && i < K_MESSAGE_SIZE; ++i)
      {
        (void)PS2_BufferPush(&input, message[i]);
      }
    }
    else if (tid != -1) switch (message[0])
    {
    case PS2_RECEIVE:
      for (i = 1; i < K_MESSAGE_SIZE &&
          PS2_BufferPull(&input, message + i); ++i);
      message[0] = (K_U8)(i - 1);
      (void)sys_send(message, tid);
      break;
    case PS2_COMMAND:
      if (!message[1] || message[1] > COMMAND_SIZE ||
          !message[2] || message[2] > COMMAND_SIZE || queue.head < 0)
      {
        message[0] = 1;
        message[1] = 0xFE;
        (void)sys_send(message, tid);
      }
      else
      {
        queue.commands[queue.head].sender = tid;
        queue.commands[queue.head].length = message[1];
        queue.commands[queue.head].returns = message[2];
        memcpy(queue.commands[queue.head++].data, message + 3, message[1]);
        if ((queue.head &= QUEUE_MASK) == queue.tail) queue.head = -1;
      }
      break;
    case PS2_DETECT:
      break;
    }
    else
    {
      if (ready)
      {
        for (i = 0; i < ready; ++i) message[i + 1] = command.data[i];
        message[0] = i;
        sys_send(message, command.sender);
        command.sender = -1;
      }
      if (command.sender == -1 && queue.head != queue.tail)
      {
        ready = 0;
        if (queue.head < 0) queue.head = queue.tail;
        command = queue.commands[queue.tail++];
        queue.tail &= QUEUE_MASK;
      }
    }
  }
  return 0;
}

