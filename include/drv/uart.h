#ifndef _DRV_UART_H
#define _DRV_UART_H

#include <simplex.h>
#include <sys/dsp.h>

#define UART_LINE_5BITS 0
#define UART_LINE_6BITS 1
#define UART_LINE_7BITS 2
#define UART_LINE_8BITS 3

#define UART_LINE_1STOP 0
#define UART_LINE_2STOP 4

#define UART_LINE_ODD   (1 << 3)
#define UART_LINE_EVEN  (3 << 3)
#define UART_LINE_MARK  (5 << 3)
#define UART_LINE_SPACE (7 << 3)

#define UART_CFG  (DSP_USER | 0)

struct UART_Cfg
{
  DSP_Header Header;
  K_U32 BaudRate;
  K_U32 LineControl;
};

typedef struct UART_Cfg UART_Cfg;

union UART_Packet
{
  DSP_Header Header;
  DSP_Packet Common;
  UART_Cfg Config;
};

typedef union UART_Packet UART_Packet;

#define UART_Send(packet) sys_send(packet, 1)

#endif

