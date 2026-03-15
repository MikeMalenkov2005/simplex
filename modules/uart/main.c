#include "uart.h"

#include <drv/dm.h>
#include <drv/uart.h>
#include <sys/memory.h>
#include <simplex.h>

struct UART_Buffer
{
  K_S16 Head, Tail;
  K_U8 Buffer[0x1000];
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
    buffer->Head &= 0xFFF;
    if (buffer->Head == buffer->Tail) buffer->Head = -1;
    return TRUE;
  }
  if (force)
  {
    buffer->Buffer[buffer->Tail++] = byte;
    buffer->Tail &= 0xFFF;
    return TRUE;
  }
  return FALSE;
}

K_BOOL UART_BufferPull(UART_Buffer *buffer, K_U8 *byte)
{
  if (buffer->Tail == buffer->Head) return FALSE;
  if (buffer->Head < 0) buffer->Head = buffer->Tail;
  *byte = buffer->Buffer[buffer->Tail++];
  buffer->Tail &= 0xFFF;
  return TRUE;
}

static UART_Buffer RxBuffer;
static UART_Buffer TxBuffer;

int main(const char *args)
{
  UART_Packet packet;
  K_U32 i;
  K_U8 byte;
  int tid;

  (void)DM_Register("UART");
  UART_BufferClear(&RxBuffer);
  UART_BufferClear(&TxBuffer);
  packet.Config.LineControl = 3;
  packet.Config.BaudRate = 115200;
  UART_SetConfig(&packet.Config);
  while (*args) UART_BufferPush(&TxBuffer, *args++, FALSE);
  UART_BufferPush(&TxBuffer, '\r', FALSE);
  UART_BufferPush(&TxBuffer, '\n', FALSE);

  while ((tid = sys_poll(&packet)))
  {
    while (UART_LineState() & 1)
    {
      (void)UART_BufferPush(&RxBuffer, UART_RxByte(), TRUE);
    }
    if (tid != -1)
    {
      switch(packet.Command)
      {
      case UART_RECEIVE:
        if (packet.Data.Size > sizeof(packet.Data.Buffer))
        {
          packet.Data.Size = sizeof(packet.Data.Buffer);
        }
        for (i = 0; i < packet.Data.Size; ++i)
        {
          if (!UART_BufferPull(&RxBuffer, packet.Data.Buffer + i)) break;
        }
        packet.Data.Size = i;
        break;
      case UART_TRANSMIT:
        if (packet.Data.Size > sizeof(packet.Data.Buffer))
        {
          packet.Data.Size = sizeof(packet.Data.Buffer);
        }
        for (i = 0; i < packet.Data.Size; ++i)
        {
          if (!UART_BufferPush(&TxBuffer, packet.Data.Buffer[i], FALSE)) break;
        }
        packet.Data.Size = i;
        break;
      case UART_GET_CONFIG:
        UART_GetConfig(&packet.Config);
        break;
      case UART_SET_CONFIG:
        UART_SetConfig(&packet.Config);
        break;
      }
      sys_fire(&packet, tid);
    }
    while ((UART_LineState() & 0x20) && UART_BufferPull(&TxBuffer, &byte))
    {
      UART_TxByte(byte);
    }
  }
  return 0;
}

