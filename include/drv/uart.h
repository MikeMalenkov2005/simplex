#ifndef _DRV_UART_H
#define _DRV_UART_H

#include <types.h>
#include <simplex.h>
#include <sys/limits.h>

#define UART_CMD_CFG  0
#define UART_CMD_RX   1
#define UART_CMD_TX   2

struct UART_CommandHeader
{
  K_U16 PortIndex; /* Ignored for now */
  K_U16 Command;
};

typedef struct UART_CommandHeader UART_CommandHeader;

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

struct UART_CfgCommand
{
  UART_CommandHeader Header;
  K_U32 BaudRate;
  K_U32 LineControl;
};

typedef struct UART_CfgCommand UART_CfgCommand;

#define UART_BYTE_COUNT_MAX (K_MESSAGE_SIZE - 8)

struct UART_RxTxCommand
{
  UART_CommandHeader Header;
  K_U32 ByteCount;
  K_U8 Bytes[UART_BYTE_COUNT_MAX];
};

typedef struct UART_RxTxCommand UART_RxTxCommand;

union UART_Command
{
  UART_CommandHeader Header;
  UART_CfgCommand Cfg;
  UART_RxTxCommand RxTx;
};

typedef union UART_Command UART_Command;

#define UART_IssueCommand(cmd)  sys_send((unsigend long)(cmd), 1)

#endif

