#include "../../ide.h"

#include <simplex.h>
#include <sys/limits.h>
#include <drv/pci.h>
#include <x86/pio.h>

K_HANDLE IDE_MapBAR(K_U32 device, K_U8 index)
{
  PCI_Packet packet = { 0 };
  packet.Command = PCI_READ_CONFIG;
  packet.Offset = PCI_OFFSET_BAR0 + (index << 2);
  packet.Device = device;
  packet.Config = ~(K_U32)0;
  sys_send(&packet, PCI);
  if (packet.Config & 1) return (K_HANDLE)packet.Config;
  return NULL; /* TODO: Implement memory mapped BARs */
}

K_U8 IDE_ReadBAR(K_HANDLE base, K_U8 offset)
{
  if (!((K_USIZE)base & 1)) return 0;
  return PIO_Read8(((K_USIZE)base & 0xFFFCU) + offset);
}

void IDE_WriteBAR(K_HANDLE base, K_U8 offset, K_U8 byte)
{
  if (!((K_USIZE)base & 1)) return;
  PIO_Write8(((K_USIZE)base & 0xFFFCU) + offset, byte);
}

