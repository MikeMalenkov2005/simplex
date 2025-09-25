#include <simplex.h>
#include <drv/uart.h>

int main()
{
  UART_Command cmd;
  cmd.Header.PortIndex = 0;
  cmd.Header.Command = UART_CMD_TX;
  cmd.RxTx.ByteCount = 7;
  cmd.RxTx.Bytes[0] = '\r';
  cmd.RxTx.Bytes[1] = '\n';
  cmd.RxTx.Bytes[2] = 'H';
  cmd.RxTx.Bytes[3] = 'e';
  cmd.RxTx.Bytes[4] = 'l';
  cmd.RxTx.Bytes[5] = 'l';
  cmd.RxTx.Bytes[6] = 'o';
  sys_send(&cmd, 1);
  for (;;);
  return 0;
}

