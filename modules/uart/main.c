#include "uart.h"

#include <drv/uart.h>
#include <sys/memory.h>

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

void UART_Reply(UART_Packet *packet, K_U8 length, int tid)
{
  packet->Header.Action = DSP_REPLY;
  packet->Header.Length = length;
  (void)sys_send(packet, tid);
}

int main(const char *args)
{
  UART_Packet packet;
  K_U32 index;
  K_U8 byte;
  int tid;

  UART_BufferClear(&RxBuffer);
  UART_BufferClear(&TxBuffer);
  UART_Config(115200, 3);
  while (*args) UART_BufferPush(&TxBuffer, *args++, FALSE);

  for (tid = -1; tid; tid = sys_poll(&packet, -1))
  {
    while (UART_LineState() & 1) (void)UART_BufferPush(&RxBuffer, UART_RxByte(), TRUE);
    if (tid != -1) switch(packet.Header.Action) /* The most simple implementation */
    {
    case UART_CFG:
      UART_Config(packet.Config.BaudRate, packet.Config.LineControl);
      UART_Reply(&packet, DSP_SUCCESS, tid);
      break;
    case DSP_READ:
      if (packet.Header.Length > sizeof packet.Common.Data) packet.Header.Length = sizeof packet.Common.Data;
      for (index = 0; index < packet.Header.Length && UART_BufferPull(&RxBuffer, packet.Common.Data.Payload + index); ++index);
      UART_Reply(&packet, index, tid);
      break;
    case DSP_WRITE:
      if (packet.Header.Length > sizeof packet.Common.Data) packet.Header.Length = sizeof packet.Common.Data;
      for (index = 0; index < packet.Header.Length && UART_BufferPush(&TxBuffer, packet.Common.Data.Payload[index], FALSE); ++index);
      UART_Reply(&packet, index, tid);
      break;
    default:
      UART_Reply(&packet, DSP_FAILURE, tid);
      break;
    }
    while ((UART_LineState() & 0x20) && UART_BufferPull(&TxBuffer, &byte)) UART_TxByte(byte);
  }
  return 0;
}

