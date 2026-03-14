#ifndef _UART_H
#define _UART_H

#include <drv/uart.h>

void UART_GetConfig(UART_Config *config);
void UART_SetConfig(const UART_Config *config);

K_U8 UART_LineState();
void UART_TxByte(K_U8 byte);
K_U8 UART_RxByte();

#endif

