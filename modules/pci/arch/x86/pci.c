#include "../../pci.h"
#include <x86/pio.h>

K_U32 PCI_ReadConfig(K_U32 device, K_U8 offset)
{
  PIO_Write32(0xCF8, (device << 8) | (offset & 0xFC) | 0x80000000);
  return PIO_Read32(0xCFC);
}

