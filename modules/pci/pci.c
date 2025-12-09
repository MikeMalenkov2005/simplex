#include "pci.h"

static K_U8 bitmap[0x2000];

K_BOOL PCI_IsDeviceValid(K_U32 device)
{
  const K_U32 index = device >> 3;
  const K_U8 mask = 1 << (device & 7);
  return index < sizeof(bitmap) && (bitmap[index] & mask);
}

#define PCI_IsMultiFunction(device) \
  ((PCI_ReadConfig(device, 12) >> 23) & 1)

static void PCI_ScanBus(K_U8 bus);

static void PCI_ScanDevice(K_U32 device)
{
  const K_U32 index = device >> 3;
  const K_U8 mask = 1 << (device & 7);
  const K_U16 eclass = PCI_ReadConfig16(device, PCI_OFFSET_SUBCLASS);
  if (index < sizeof(bitmap)) bitmap[index] |= mask;
  if (eclass == 0x0604) PCI_ScanBus(PCI_ReadConfig16(device, 30));
}

static void PCI_ScanSlot(K_U8 bus, K_U8 slot)
{
  K_U32 device = PCI_Device(bus, slot, 0);
  K_U16 vendor = PCI_ReadConfig16(device, PCI_OFFSET_VENDOR_ID);
  if (vendor != 0xFFFFU)
  {
    PCI_ScanDevice(device);
    if (PCI_IsMultiFunction(device++)) while (device & 3)
    {
      vendor = PCI_ReadConfig16(device, PCI_OFFSET_VENDOR_ID);
      if (vendor != 0xFFFFU) PCI_ScanDevice(device);
      ++device;
    }
  }
}

static void PCI_ScanBus(K_U8 bus)
{
  K_U8 slot = 0;
  while (slot < 32) PCI_ScanSlot(bus, slot);
}

void PCI_ScanDevices(void)
{
  K_U16 vendor;
  K_U8 bus = 0;
  if (PCI_IsMultiFunction(0)) while (bus < 8)
  {
    vendor = PCI_ReadConfig16(bus, PCI_OFFSET_VENDOR_ID);
    if (vendor != 0xFFFFU) PCI_ScanBus(bus);
    ++bus;
  }
  else PCI_ScanBus(0);
}

void PCI_EnumerateDevices(void (*callback)(K_U32, K_HANDLE), K_HANDLE context)
{
  K_U8 shift;
  K_U32 index = 0;
  if (callback) while (index < (K_U32)sizeof(bitmap))
  {
    if (bitmap[index]) for (shift = 0; shift < 8; ++shift)
    {
      if (bitmap[index] & (1 << shift)) callback((index << 3) | shift, context);
    }
    ++index;
  }
}

