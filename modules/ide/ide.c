#include "ide.h"

#include <drv/pci.h>
#include <sys/pio.h>

#define MAX_DEVICE_COUNT  16

static IDE_Device devices[MAX_DEVICE_COUNT];
static K_U32 device_count;

static K_BOOL IDE_IsDrive(K_U32 drive)
{
  IDE_Device *device = devices + (drive >> 3);
  IDE_Drive *p = device->Drive + ((drive >> 1) & 3);
  return (drive >> 3) < device_count && p->Drive == drive;
}

static void IDE_SetByte(K_U32 drive, K_U8 reg, K_U8 val)
{
  int channel = !!(drive & IDE_CHANNEL_MASK);
  IDE_Device *device = devices + (drive >> 3);
  if (reg > 0x07 && reg < 0x0C)
    IDE_SetByte(drive, ATA_REG_CONTROL, 0x80 | device->NIEN[channel]);
  if (reg < 0x08)
    PIO_Write8(device->BAR[channel << 1] + reg, val);
  else if (reg < 0x0C)
    PIO_Write8(device->BAR[channel << 1] + reg - 0x06, val);
  else if (reg < 0x0E)
    PIO_Write8(device->BAR[(channel << 1) | 1] + reg - 0x0A, val);
  else if (reg < 0x16)
    PIO_Write8(device->BAR[4] + (channel << 3) + reg - 0x0E, val);
  if (reg > 0x07 && reg < 0x0C)
    IDE_SetByte(drive, ATA_REG_CONTROL, device->NIEN[channel]);
}

static K_U8 IDE_GetByte(K_U32 drive, K_U8 reg)
{
  K_U8 result = 0;
  int channel = !!(drive & IDE_CHANNEL_MASK);
  IDE_Device *device = devices + (drive >> 3);
  if (reg > 0x07 && reg < 0x0C)
    IDE_SetByte(drive, ATA_REG_CONTROL, 0x80 | device->NIEN[channel]);
  if (reg < 0x08)
    result = PIO_Read8(device->BAR[channel << 1] + reg);
  else if (reg < 0x0C)
    result = PIO_Read8(device->BAR[channel << 1] + reg - 0x06);
  else if (reg < 0x0E)
    result = PIO_Read8(device->BAR[(channel << 1) | 1] + reg - 0x0A);
  else if (reg < 0x16)
    result = PIO_Read8(device->BAR[4] + (channel << 3) + reg - 0x0E);
  if (reg > 0x07 && reg < 0x0C)
    IDE_SetByte(drive, ATA_REG_CONTROL, device->NIEN[channel]);
  return result;
}

static void IDE_InitDrive(K_U32 drive)
{
  IDE_Device *device = devices + (drive >> 3);
  device->Drive[(drive >> 1) & 3].Drive = -1;
}

static void IDE_InitDevice(int device)
{
  K_U32 i, idx = device_count;
  IDE_Device *p = devices + device_count;
  if (device_count >= MAX_DEVICE_COUNT) return;
  p->Device = device;
  for (i = 0; i < 5; ++i)
  {
    p->BAR[i] = PCI_Read(device, PCI_OFFSET_BAR0 + i * 4);
    if (!(p->BAR[i] & 1)) return; /* IDE does not support MMIO. */
    p->BAR[i] &= 0xFFFCU;
  }
  for (i = 0; i < 4; ++i)
  {
    IDE_SetByte((idx << 3) | IDE_CHANNEL_PRIMARY, ATA_REG_CONTROL, 2);
    IDE_SetByte((idx << 3) | IDE_CHANNEL_SECONDARY, ATA_REG_CONTROL, 2);
    IDE_InitDrive((idx << 3) | (i << 1));
  }
  for (i = 0; i < 4; ++i) if (~p->Drive[i].Drive)
  {
    ++device_count;
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

