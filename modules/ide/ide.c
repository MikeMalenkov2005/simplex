#include "ide.h"

#include <drv/dm.h>
#include <drv/uart.h>
#include <simplex.h>

static void logch(int ch)
{
  static int uart = -1;
  UART_Packet packet;
  if (uart == -1) uart = DM_Wait("UART", 512);
  packet.Command = UART_TRANSMIT;
  packet.Data.Size = 1;
  packet.Data.Buffer[0] = ch;
  sys_send(&packet, uart);
}

static void logu8(K_U8 u8)
{
  logch(((u8 >> 4) & 15) + (((u8 >> 4) & 15) < 10 ? '0' : 'A' - 10));
  logch(((u8 >> 0) & 15) + (((u8 >> 0) & 15) < 10 ? '0' : 'A' - 10));
}

static void logu16(K_U16 u16)
{
  logu8(u16 >> 8);
  logu8(u16 >> 0);
}

static void logu32(K_U32 u32)
{
  logu16(u32 >> 16);
  logu16(u32 >>  0);
}

void IDE_Init(K_U32 device)
{
  logu32(device);
  logch('\r');
  logch('\n');
}

