#include "../../uart.h"
#include <sys/pio.h>

#define COM1  0x3F8
#define COM2  0x2F8
#define COM3  0x3E8
#define COM4  0x2E8

#define THR 0 /* Transmitter Holding Buffer         */
#define RBR 0 /* Receiver Buffer                    */
#define DLL 0 /* Divisor Latch Low Byte             */
#define IER 1 /* Interrupt Enable Register          */
#define DLH 1 /* Divisor Latch High Byte            */
#define IIR 2 /* Interrupt Identification Register  */
#define FCR 2 /* FIFO Control Register              */
#define LCR 3 /* Line Control Register              */
#define MCR 4 /* Modem Control Register             */
#define LSR 5 /* Line Status Register               */
#define MSR 6 /* Modem Status Register              */
#define SR  7 /* Scratch Register                   */

static K_U32 BaudRate;
static K_U8 LineControl;

void UART_GetConfig(UART_Config *config)
{
  if (!config) return;
  config->BaudRate = BaudRate;
  config->LineControl = LineControl;
}

void UART_SetConfig(const UART_Config *config)
{
  K_U32 divisor;
  if (!config) return;
  BaudRate = config->BaudRate;
  LineControl = config->LineControl;
  divisor = BaudRate ? 115200 / BaudRate : 1;
  PIO_Write8(COM1 + LCR, LineControl | 0x80);
  PIO_Write8(COM1 + DLL, (K_U8)divisor);
  PIO_Write8(COM1 + DLH, (K_U8)(divisor >> 8));
  PIO_Write8(COM1 + LCR, LineControl & 0x7F);
  PIO_Write8(COM1 + IER, 0);
  PIO_Write8(COM1 + FCR, 7);
}

K_U8 UART_LineState()
{
  return PIO_Read8(COM1 + LSR);
}

void UART_TxByte(K_U8 byte)
{
  PIO_Write8(COM1 + THR, byte);
}

K_U8 UART_RxByte()
{
  return PIO_Read8(COM1 + RBR);
}

