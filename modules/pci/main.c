#include "pci.h"

#include <drv/dm.h>
#include <drv/uart.h>
#include <simplex.h>

static void logch(int ch)
{
  static int uart = -1;
  UART_Packet packet;
  if (uart == -1)
  {
    uart = DM_Find("UART");
    if (uart == -1) sys_switch(), uart = DM_Find("UART");
    if (uart == -1) sys_switch(), uart = DM_Find("UART");
    if (uart == -1) sys_switch(), uart = DM_Find("UART");
    if (uart == -1) uart = 2;
  }
  packet.Command = UART_TRANSMIT;
  packet.Data.Size = 1;
  packet.Data.Buffer[0] = ch;
  sys_send(&packet, uart);
}

static void logu8(K_U8 u8)
{
  logch(((u8 >> 4) & 15) + (((u8 >> 4) & 15) < 10 ? '0' : 'A' - 10));
  logch(((u8 >> 0) & 15) + (((u8 >> 0) & 15) < 10 ? '0' : 'A' - 10));
}

static void logu16(K_U16 u16)
{
  logu8(u16 >> 8);
  logu8(u16 >> 0);
}

static void logu32(K_U32 u32)
{
  logu16(u32 >> 16);
  logu16(u32 >>  0);
}

static void logdev(K_U32 device, K_HANDLE ignored)
{
  K_U32 r0, r2;
  (void)ignored;
  r0 = PCI_ReadConfig(device, PCI_OFFSET_VENDOR_ID);
  r2 = PCI_ReadConfig(device, PCI_OFFSET_REVISION);
  logu32(device);
  logch(' ');
  logu16(r0);
  logch(':');
  logu16(r0 >> 16);
  logch(' ');
  logu8(r2 >> 24);
  logch('.');
  logu8(r2 >> 16);
  logch('.');
  logu8(r2 >> 8);
  logch('.');
  logu8(r2);
  logch('\r');
  logch('\n');
}

static void PCI_FilterDevice(K_U32 device, K_HANDLE context)
{
  K_U32 r0, r2;
  PCI_Packet *packet = context;
  if (packet->Device != PCI_INVALID_DEVICE) return;
  r0 = PCI_ReadConfig(device, PCI_OFFSET_VENDOR_ID);
  if (~packet->VendorID && packet->VendorID != (K_U16)(r0 >> 0)) return;
  if (~packet->DeviceID && packet->DeviceID != (K_U16)(r0 >> 16)) return;
  r2 = PCI_ReadConfig(device, PCI_OFFSET_REVISION);
  if (~packet->Interface && packet->Interface != ((r2 >> 8) & 255)) return;
  if (~packet->Subclass && packet->Subclass != ((r2 >> 16) & 255)) return;
  if (~packet->Class && packet->Class != ((r2 >> 24) & 255)) return;
  if (packet->Offset) --packet->Offset;
  else packet->Device = device;
}

int main(void)
{
  PCI_Packet packet;
  int tid;
  if (!DM_Register("PCI")) return 1;
  PCI_ScanDevices();
  PCI_EnumerateDevices(logdev, NULL);
  while ((tid = sys_wait(&packet)) != -1)
  {
    switch (packet.Command)
    {
    case PCI_READ_CONFIG:
      packet.Config = PCI_ReadConfig(packet.Device, packet.Offset);
      packet.Config >>= ((packet.Offset & 3) << 3);
      break;
    case PCI_FIND_DEVICE:
      packet.Device = PCI_INVALID_DEVICE;
      PCI_EnumerateDevices(PCI_FilterDevice, &packet);
      break;
    }
    sys_fire(&packet, tid);
  }
  return 0;
}

