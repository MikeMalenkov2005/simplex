#ifndef _UART_H
#define _UART_H

#include <types.h>

void UART_Config(K_U32 baud, K_U8 lcr);

K_U8 UART_LineState();
void UART_TxByte(K_U8 byte);
K_U8 UART_RxByte();

#endif

