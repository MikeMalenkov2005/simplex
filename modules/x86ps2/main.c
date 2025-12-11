#include "ps2.h"

#include <drv/x86ps2.h>
#include <sys/limits.h>
#include <simplex.h>
#include <x86/pio.h>

static PS2_Buffer input;
static volatile K_U8 answer, result, waiting;

extern int __crt_atomic_compare_swap(volatile int *var, int *exp, int val);

K_BOOL PS2_BufferPull(PS2_Buffer *buffer, K_U8 *byte)
{
  int info = -1;
  int new_info = 0;
  short head, tail;
  while (!__crt_atomic_compare_swap(&buffer->info, &info, new_info))
  {
    head = (short)(info >> 0);
    tail = (short)(info >> 16);
    if (head == tail) return FALSE;
    if (byte) *byte = buffer->data[head];
    if (tail < 0) tail = head;
    head = (head + 1) & BUFFER_MASK;
    new_info = (int)head | ((int)tail << 16);
  }
  return TRUE;
}

void PS2_BufferPush(PS2_Buffer *buffer, K_U8 byte)
{
  int info = -1;
  int new_info = 0;
  short head, tail;
  while (!__crt_atomic_compare_swap(&buffer->info, &info, new_info))
  {
    head = (short)(info >> 0);
    tail = (short)(info >> 16);
    if (tail < 0)
    {
      buffer->data[head++] = byte;
      head &= BUFFER_MASK;
    }
    else
    {
      buffer->data[tail++] = byte;
      if ((tail &= BUFFER_MASK) == head) tail = -1;
    }
    new_info = (int)head | ((int)tail << 16);
  }
}

K_BOOL PS2_SendByte1(K_U16 timeout, K_U8 byte)
{
  while (timeout-- && !(PIO_Read8(0x64) & 2)) sys_switch();
  if (timeout) PIO_Write8(0x60, byte);
  return timeout;
}

void irq_handler()
{
  K_U8 byte;

  while (sys_irq_wait(1) != -1) while (PIO_Read8(0x64) & 1)
  {
    byte = PIO_Read8(0x60);
    if (waiting && answer)
    {
      result = byte;
      waiting = 0;
    }
    else if (waiting && (byte == 0xFA || byte == 0xFE || byte == 0xEE))
    {
      answer = byte;
      if (byte != 0xFA) result = 0;
      waiting = result;
    }
    else
    {
      PS2_BufferPush(&input, byte);
    }
  }

  sys_exit(0);
}

void test_print(int ch)
{
  K_U8 msg[K_MESSAGE_SIZE] = "\002";
  msg[1] = ch;
  sys_send(msg, 3);
}

int main(void)
{
  K_U8 msg[K_MESSAGE_SIZE];
  int tid = -1;
  K_U16 to;
  if (sys_thread(irq_handler, 0) == -1) return 1;
  while ((tid = sys_wait(msg)) != -1)
  {
    switch (msg[0])
    {
    case PS2_RECEIVE:
      msg[0] = 1;
      if (!msg[1]) while (msg[0] < K_MESSAGE_SIZE
          && PS2_BufferPull(&input, msg + msg[0])) ++msg[0];
      --msg[0];
      break;
    case PS2_COMMAND:
      to = 500;
      result = msg[1];
      if (PS2_SendByte1(to, msg[2]) && (!~msg[3] || PS2_SendByte1(to, msg[3])))
      {
        waiting = 1;
        while (waiting && to--) sys_switch();
        waiting = 0;
        if (to)
        {
          msg[0] = answer;
          msg[1] = result;
        }
        else msg[0] = 0xFE;
      }
      else msg[0] = 0xFE;
      break;
    }
    sys_fire(msg, tid);
  }
  return 0;
}

