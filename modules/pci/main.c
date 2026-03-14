#include "pci.h"

#include <simplex.h>
#include <drv/uart.h>

void logch(K_U8 ch)
{
  UART_Packet packet = { 0 };
  packet.Command = UART_TRANSMIT;
  packet.Data.Size = 1;
  packet.Data.Buffer[0] = ch;
  sys_send(&packet, UART_TASK_ID);
}

void logu8(K_U8 u8)
{
  logch(((u8 >> 4) & 15) + (((u8 >> 4) & 15) < 10 ? '0' : 'A' - 10));
  logch(((u8 >> 0) & 15) + (((u8 >> 0) & 15) < 10 ? '0' : 'A' - 10));
}

void logu16(K_U16 u16)
{
  logu8(u16 >> 8);
  logu8(u16 >> 0);
}

void logu32(K_U32 u32)
{
  logu16(u32 >> 16);
  logu16(u32 >>  0);
}

void logdev(K_U32 device, K_HANDLE ignored)
{
  (void)ignored;
  logu32(device);
  logch(' ');
  logu16(PCI_ReadConfig16(device, PCI_OFFSET_VENDOR_ID));
  logch(':');
  logu16(PCI_ReadConfig16(device, PCI_OFFSET_DEVICE_ID));
  logch(' ');
  logu8(PCI_ReadConfig8(device, PCI_OFFSET_CLASS));
  logch('.');
  logu8(PCI_ReadConfig8(device, PCI_OFFSET_SUBCLASS));
  logch('.');
  logu8(PCI_ReadConfig8(device, PCI_OFFSET_INTERFACE));
  logch(' ');
  logu32(PCI_ReadConfig32(device, PCI_OFFSET_BAR0));
  logch(' ');
  logu32(PCI_ReadConfig32(device, PCI_OFFSET_BAR1));
  logch('\r');
  logch('\n');
}

int main(void)
{
  K_U8 msg[K_MESSAGE_SIZE];
  int tid;
  PCI_ScanDevices();
  PCI_EnumerateDevices(logdev, NULL);
  while ((tid = sys_wait(msg)) != -1)
  {
  }
  return 0;
}

