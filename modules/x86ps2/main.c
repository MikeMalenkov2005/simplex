#include <drv/x86ps2.h>
#include <sys/limits.h>
#include <simplex.h>
#include <x86/pio.h>

#define BUFFER_SIZE 512

typedef struct PS2_Buffer
{
  int head, tail;
  K_U8 data[BUFFER_SIZE];
} PS2_Buffer;

static PS2_Buffer input;

K_BOOL PS2_BufferPull(PS2_Buffer *buffer, K_U8 *byte)
{
  if (!buffer || buffer->tail == buffer->head) return FALSE;
  if (byte) *byte = buffer->data[buffer->tail];
  if (buffer->head < 0) buffer->head = buffer->tail;
  if (++buffer->tail > BUFFER_SIZE) buffer->tail = 0;
  return TRUE;
}

K_BOOL PS2_BufferPush(PS2_Buffer *buffer, K_U8 byte)
{
  if (!buffer || buffer->head < 0) return FALSE;
  buffer->data[buffer->head++] = byte;
  if (buffer->head > BUFFER_SIZE) buffer->head = 0;
  if (buffer->head == buffer->tail) buffer->head = -1;
  return TRUE;
}

void irq_handler()
{
  while (sys_irq_wait(1) != -1)
  {
  }
}

int main()
{
  /* TODO: Create a IRQ handler thread! */
  int pid, i;
  K_U8 message[K_MESSAGE_SIZE];
  while ((pid = sys_wait(message, -1))) if (pid != -1) switch (message[0])
  {
  case PS2_NEXT_CHUNK:
    for (i = 1; i < K_MESSAGE_SIZE && PS2_BufferPull(&input, message + i); ++i);
    message[0] = (K_U8)i;
    sys_send(message, pid);
    break;
  }
  return 0;
}

