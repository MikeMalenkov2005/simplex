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

K_BOOL PS2_BufferPull(volatile PS2_Buffer *buffer, K_U8 *byte)
{
  int head, tail;
  if (!buffer) return FALSE;
  do {
    head = buffer->head;
    tail = buffer->tail;
    buffer->changed = 0;
    if (head == tail) return FALSE;
    if (byte) *byte = buffer->data[tail];
  } while (buffer->changed);
  if (head < 0) buffer->head = tail;
  buffer->tail = (tail + 1) & BUFFER_MASK;
  return TRUE;
}

void irq_handler()
{
  static K_U8 left = 0;
  K_U8 byte;

  while (sys_irq_wait(1) != -1) while (PIO_Read8(0x64) & 1)
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
    else if (input.head < 0)
    {
      input.data[input.tail++] = byte;
      input.tail &= BUFFER_MASK;
      input.changed = 1;
    }
    else
    {
      input.data[input.head++] = byte;
      if ((input.head &= BUFFER_MASK) == input.tail)
      {
        input.head = -1;
        input.changed = 1;
      }
    }
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
      /* TODO: use messages to move input get irq handler */
    }
    if (tid != -1) switch (message[0])
    {
    case PS2_RECEIVE:
      for (i = 1; i < K_MESSAGE_SIZE &&
          PS2_BufferPull(&input, message + i); ++i);
      message[0] = (K_U8)i;
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

