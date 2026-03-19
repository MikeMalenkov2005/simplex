#include <drv/dm.h>
#include <drv/uart.h>
#include <simplex.h>

static int UART = -1;

int UART_RxChar(void)
{
  static K_U8 idx;
  static UART_Packet packet;
  if (UART == -1 && (UART = DM_Wait("UART", 128)) == -1) return -1;
  while (idx >= packet.Data.Size)
  {
    idx = 0;
    packet.Command = UART_RECEIVE;
    packet.Data.Size = 255;
    sys_send(&packet, UART);
    if (packet.Data.Size == 255)
    {
      packet.Data.Size = 0;
      return -1;
    }
  }
  return packet.Data.Buffer[idx++];
}

int UART_TxChar(int ch)
{
  UART_Packet packet = { UART_TRANSMIT };
  if (UART == -1 && (UART = DM_Wait("UART", 128)) == -1) return -1;
  packet.Data.Size = 1;
  packet.Data.Buffer[0] = ch;
  return -(sys_send(&packet, UART) == -1 || !packet.Data.Size);
}

