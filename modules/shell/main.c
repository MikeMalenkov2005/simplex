#include <simplex.h>
#include <drv/uart.h>

int main()
{
  DSP_Message msg = { 0 };
  msg.Header.Flags = DSP_TX;
  msg.Header.Bytes = 9;
  msg.Data[0] = '\r';
  msg.Data[1] = '\n';
  msg.Data[2] = 'H';
  msg.Data[3] = 'e';
  msg.Data[4] = 'l';
  msg.Data[5] = 'l';
  msg.Data[6] = 'o';
  msg.Data[7] = '\r';
  msg.Data[8] = '\n';
  UART_Send(&msg);
  for (;;);
  return 0;
}

