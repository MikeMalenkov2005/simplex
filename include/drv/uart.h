#ifndef _DRV_UART_H
#define _DRV_UART_H

#include <sys/limits.h>
#include <sys/types.h>

#define UART_RECEIVE    1
#define UART_TRANSMIT   2
#define UART_GET_CONFIG 3
#define UART_SET_CONFIG 4

#define UART_RECEIVE_S    "\001"
#define UART_TRANSMIT_S   "\002"
#define UART_GET_CONFIG_S "\003"
#define UART_SET_CONFIG_S "\004"

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

struct UART_Config
{
  K_U8 Command;
  K_U8 LineControl;
  K_U16 Reserved;
  K_U32 BaudRate;
  K_U8 Padding[K_MESSAGE_SIZE - 8];
};

typedef struct UART_Config UART_Config;

struct UART_Data
{
  K_U8 Command;
  K_U8 Size;
  K_U8 Buffer[K_MESSAGE_SIZE - 2];
};

typedef struct UART_Data UART_Data;

union UART_Packet
{
  K_U8 Command;
  UART_Data Data;
  UART_Config Config;
};

typedef union UART_Packet UART_Packet;

/**
 * @return The character received on success or -1 on failure.
 */
int UART_RxChar(void);

/**
 * @param ch The character to transmit.
 * @return 0 on success or -1 on failure.
 */
int UART_TxChar(int ch);

#endif

