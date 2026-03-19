#include "../../bar.h"

#include <drv/pci.h>
#include <x86/pio.h>

HBAR BAR_Init(int device, int index)
{
  int cfg = PCI_Read(device, PCI_OFFSET_BAR0 + (index << 2));
  if (cfg & 1) return cfg;
  return 0; /* TODO: Implement memory mapped BARs */
}

void BAR_Free(HBAR base)
{
  (void)base;
}

int BAR_GetByte(HBAR base, unsigned offset)
{
  if (!((K_USIZE)base & 1)) return -1;
  return PIO_Read8(((K_USIZE)base & 0xFFFCU) + offset);
}

int BAR_SetByte(HBAR base, unsigned offset, int value)
{
  if (!((K_USIZE)base & 1)) return -1;
  PIO_Write8(((K_USIZE)base & 0xFFFCU) + offset, value);
  return 0;
}

