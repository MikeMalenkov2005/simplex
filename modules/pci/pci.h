#ifndef _PCI_H
#define _PCI_H

#include <drv/pci.h>

K_U32 PCI_ReadConfig(K_U32 device, K_U8 offset);

#define PCI_ReadConfig32 PCI_ReadConfig

#define PCI_ReadConfig16(device, offset)  \
  ((K_U16)(PCI_ReadConfig32((device), offset) >> (((offset) & 2) << 3)))

#define PCI_ReadConfig8(device, offset)  \
  ((K_U8)(PCI_ReadConfig32((device), (offset)) >> (((offset) & 3) << 3)))

K_BOOL PCI_IsDeviceValid(K_U32 device);

void PCI_ScanDevices(void);

void PCI_EnumerateDevices(void (*callback)(K_U32, K_HANDLE), K_HANDLE context);

#endif

