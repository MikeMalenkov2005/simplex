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

K_BOOL PS2_WriteCmd(K_U16 timeout, K_U8 cmd)
{
  while ((PIO_Read8(0x64) & 2) && timeout && --timeout) sys_switch();
  if (timeout) PIO_Write8(0x64, cmd);
  return timeout;
}

K_BOOL PS2_WriteData(K_U16 timeout, K_U8 byte)
{
  while ((PIO_Read8(0x64) & 2) && timeout && --timeout) sys_switch();
  if (timeout) PIO_Write8(0x60, byte);
  return timeout;
}

K_BOOL PS2_WaitData(K_U16 timeout, K_U8 *byte)
{
  if (!byte) return FALSE;
  while (!(PIO_Read8(0x64) & 1) && timeout && --timeout) sys_switch();
  if (timeout) *byte = PIO_Read8(0x60);
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

#include <drv/x86vga.h>

void putch(int ch)
{
  K_U8 output[K_MESSAGE_SIZE] = VGA_PUT_SYMBOL_S;
  output[1] = ch;
  sys_send(output, VGA_TASK_ID);
}

K_BOOL PS2_Init(void)
{
  K_U8 data, dual_port, failed[2];
  /* Disable devices */
  if (!PS2_WriteCmd(50, 0xAD)) return FALSE;
  if (!PS2_WriteCmd(50, 0xA7)) return FALSE;
  /* Flush the output buffer */
  while (PIO_Read8(0x64) & 1) (void)PIO_Read8(0x60);
  /* Modify controller configuration (disable first port and translation) */
  if (!PS2_WriteCmd(50, 0x20)) return FALSE;
  if (!PS2_WaitData(50, &data)) return FALSE;
  if (!PS2_WriteCmd(50, 0x60)) return FALSE;
  if (!PS2_WriteData(50, data & 0xAE)) return FALSE;
  /* Perform the controller self test */
  if (!PS2_WriteCmd(50, 0xAA)) return FALSE; /* FIX: Failes if holding a key */
  if (!PS2_WaitData(50, &data) || data != 0x55) return FALSE;
  /* Test for the second port capability */
  if (!PS2_WriteCmd(50, 0xA8)) return FALSE;
  if (!PS2_WriteCmd(50, 0x20)) return FALSE;
  if (!PS2_WaitData(50, &data)) return FALSE;
  if ((dual_port = !((data &= 0xAE) & 0x20)))
  {
    /* Disable second port */
    if (!PS2_WriteCmd(50, 0xA7)) return FALSE;
    if (!PS2_WriteCmd(50, 0x60)) return FALSE;
    if (!PS2_WriteData(50, data &= 0x8C)) return FALSE;
  }
  /* Test the first port */
  if (!PS2_WriteCmd(50, 0xAB)) return FALSE;
  if (!PS2_WaitData(50, failed + 0)) return FALSE;
  if (dual_port)
  {
    /* Test the second port */
    if (!PS2_WriteCmd(50, 0xA9)) return FALSE;
    if (!PS2_WaitData(50, failed + 1)) return FALSE;
  }
  else failed[1] = -1;
  data |= !failed[0] | (!failed[1] << 1);
  /* Modify controller configuration (enable IRQ for not failed ports) */
  if (!PS2_WriteCmd(50, 0x60)) return FALSE;
  if (!PS2_WriteData(50, data)) return FALSE;
  /* Enable devices */
  if (!failed[0] && !PS2_WriteCmd(50, 0xAE)) return FALSE;
  if (!failed[1] && !PS2_WriteCmd(50, 0xA8)) return FALSE;
  return !failed[0] || !failed[1];
}

int main(void)
{
  K_U8 msg[K_MESSAGE_SIZE];
  int tid = -1, to;
  if (!PS2_Init()) return 1;
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
      if (PS2_WriteData(50, msg[2]) && (!~msg[3] || PS2_WriteData(50, msg[3])))
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

