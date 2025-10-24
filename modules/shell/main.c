#include <simplex.h>
#include <drv/uart.h>

int main()
{
  DSP_Packet packet = { 0 };
  packet.Header.Action = DSP_WRITE;
  packet.Header.Length = 9;
  packet.Data.Payload[0] = '\r';
  packet.Data.Payload[1] = '\n';
  packet.Data.Payload[2] = 'H';
  packet.Data.Payload[3] = 'e';
  packet.Data.Payload[4] = 'l';
  packet.Data.Payload[5] = 'l';
  packet.Data.Payload[6] = 'o';
  packet.Data.Payload[7] = '\r';
  packet.Data.Payload[8] = '\n';
  UART_Send(&packet);
  if (UART_Wait(&packet) != -1)
  {
  }
  for (;;);
  return 0;
}

