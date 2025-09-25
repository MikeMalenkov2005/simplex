#include "uart.h"

#include <drv/uart.h>
#include <sys/memory.h>

struct UART_Buffer
{
  K_S16 Head, Tail;
  K_U8 Buffer[128];
};

typedef struct UART_Buffer UART_Buffer;

void UART_BufferClear(UART_Buffer *buffer)
{
  buffer->Head = 0;
  buffer->Tail = 0;
}

K_BOOL UART_BufferPush(UART_Buffer *buffer, K_U8 byte, K_BOOL force)
{
  if (buffer->Head >= 0)
  {
    buffer->Buffer[buffer->Head++] = byte;
    buffer->Head &= 127;
    if (buffer->Head == buffer->Tail) buffer->Head = -1;
    return TRUE;
  }
  if (force)
  {
    buffer->Buffer[buffer->Tail++] = byte;
    buffer->Tail &= 127;
    return TRUE;
  }
  return FALSE;
}

K_BOOL UART_BufferPull(UART_Buffer *buffer, K_U8 *byte)
{
  if (buffer->Tail == buffer->Head) return FALSE;
  if (buffer->Head < 0) buffer->Head = buffer->Tail;
  *byte = buffer->Buffer[buffer->Tail++];
  buffer->Tail &= 127;
  return TRUE;
}

static UART_Buffer RxBuffer;
static UART_Buffer TxBuffer;

void UART_Main(const char *args)
{
  UART_CfgData *cfg;
  int tid;
  DSP_Message msg;
  K_U32 index;
  K_U8 byte;

  UART_BufferClear(&RxBuffer);
  UART_BufferClear(&TxBuffer);
  UART_Config(9600, 3);
  while (*args) UART_BufferPush(&TxBuffer, *args++, FALSE);

  for (tid = -1; tid; tid = sys_poll(&msg))
  {
    if (tid == -1)
    {
      while (UART_LineState() & 1) (void)UART_BufferPush(&RxBuffer, UART_RxByte(), TRUE);
      while ((UART_LineState() & 0x20) && UART_BufferPull(&TxBuffer, &byte)) UART_TxByte(byte);
    }
    else switch(msg.Header.Flags) /* The most simple implementation */
    {
    case DSP_CFG:
      cfg = (void*)msg.Data;
      UART_Config(cfg->BaudRate, cfg->LineControl);
      msg.Header.Flags |= DSP_ANS;
      (void)sys_send(&msg, tid);
      break;
    case DSP_RX:
      if (msg.Header.Bytes > sizeof msg.Data) msg.Header.Bytes = sizeof msg.Data;
      for (index = 0; index < msg.Header.Bytes && UART_BufferPull(&RxBuffer, msg.Data + index); ++index);
      msg.Header.Bytes = index;
      msg.Header.Flags |= DSP_ANS;
      (void)sys_send(&msg, tid);
      break;
    case DSP_TX:
      if (msg.Header.Bytes > sizeof msg.Data) msg.Header.Bytes = sizeof msg.Data;
      for (index = 0; index < msg.Header.Bytes && UART_BufferPush(&TxBuffer, msg.Data[index], FALSE); ++index);
      msg.Header.Bytes = index;
      msg.Header.Flags |= DSP_ANS;
      (void)sys_send(&msg, tid);
      break;
    default:
      msg.Header.Flags |= DSP_ERR | DSP_ANS;
      (void)sys_send(&msg, tid);
      break;
    }
  }
}

