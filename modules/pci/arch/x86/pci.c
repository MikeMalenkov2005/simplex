#include "../../pci.h"
#include <x86/pio.h>

K_U32 PCI_ReadConfig(K_U32 device, K_U8 offset)
{
  if (device > 0xFFFFU) return ~(K_U32)0;
  PIO_Write32(0xCF8, (device << 8) | (offset & 0xFC) | ((K_U32)1 << 31));
  return PIO_Read32(0xCFC);
}

