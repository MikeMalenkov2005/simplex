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

struct UART_CfgData
{
  K_U32 BaudRate;
  K_U32 LineControl;
};

typedef struct UART_CfgData UART_CfgData;

#define UART_Send(msg)  sys_send(msg, 1)

#endif

