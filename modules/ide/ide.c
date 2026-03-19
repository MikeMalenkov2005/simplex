#include "ide.h"

#include <drv/pci.h>

#define MAX_DEVICE_COUNT  16

static IDE_Device devices[MAX_DEVICE_COUNT];
static K_U32 device_count;

static IDE_Drive drives[MAX_DEVICE_COUNT << 2];
static K_U32 drive_count;

static void IDE_InitDevice(int device)
{
  int i;
  IDE_Device *p = devices + device_count;
  if (device_count >= MAX_DEVICE_COUNT) return;
  for (i = 0; i < 5; ++i) if (!(p->BAR[i] = BAR_Init(device, i)))
  {
    for (i = 0; i < 5; ++i)
    {
      BAR_Free(p->BAR[i]);
      p->BAR[i] = 0;
    }
    return;
  }
}

void IDE_Init(void)
{
  int device, offset = 0;
  while ((device = PCI_Find(-1, PCI_Type(1, 1, -1), offset++)) != -1)
  {
    IDE_InitDevice(device);
  }
}

