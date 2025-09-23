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
  int tid;
  UART_Command cmd;
  K_U32 index;
  K_U8 byte;

  UART_BufferClear(&RxBuffer);
  UART_BufferClear(&TxBuffer);
  UART_Config(9600, 3);
  while (*args) UART_BufferPush(&TxBuffer, *args++, FALSE);

  for (tid = -1; tid; tid = sys_poll(&cmd))
  {
    if (tid == -1)
    {
      while (UART_LineState() & 1) (void)UART_BufferPush(&RxBuffer, UART_RxByte(), TRUE);
      while ((UART_LineState() & 0x20) && UART_BufferPull(&TxBuffer, &byte)) UART_TxByte(byte);
    }
    else switch(cmd.Header.Command)
    {
    case UART_CMD_CFG:
      UART_Config(cmd.Cfg.BaudRate, cmd.Cfg.LineControl);
      break;
    case UART_CMD_RX:
      if (cmd.RxTx.ByteCount > UART_BYTE_COUNT_MAX) cmd.RxTx.ByteCount = UART_BYTE_COUNT_MAX;
      for (index = 0; index < cmd.RxTx.ByteCount && UART_BufferPull(&RxBuffer, cmd.RxTx.Bytes + index); ++index);
      cmd.RxTx.ByteCount = index;
      (void)sys_send(&cmd, tid);
      break;
    case UART_CMD_TX:
      if (cmd.RxTx.ByteCount > UART_BYTE_COUNT_MAX) cmd.RxTx.ByteCount = UART_BYTE_COUNT_MAX;
      for (index = 0; index < cmd.RxTx.ByteCount && UART_BufferPush(&TxBuffer, cmd.RxTx.Bytes[index], FALSE); ++index);
      cmd.RxTx.ByteCount = index;
      (void)sys_send(&cmd, tid);
      break;
    }
  }
}

