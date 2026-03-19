#ifndef _IDE_H
#define _IDE_H

#include "ata.h"

#include <drv/ide.h>

#define IDE_CHANNEL_MASK      4
#define IDE_CHANNEL_PRIMARY   0
#define IDE_CHANNEL_SECONDARY 4

#define IDE_DRIVE_MASK    2
#define IDE_DRIVE_MASTER  0
#define IDE_DRIVE_SLAVE   2

#define IDE_TYPE_MASK   1
#define IDE_TYPE_ATA    0
#define IDE_TYPE_ATAPI  1

struct IDE_Drive
{
  K_USIZE Size;
  K_U32 Drive;
  K_U16 Features;
  K_U16 Signature;
  K_U32 CommandSets;
  K_U8 Model[48];
};

typedef struct IDE_Drive IDE_Drive;

struct IDE_Device
{
  int Device;
  K_U16 BAR[5];
  K_U8 NIEN[2];
  IDE_Drive Drive[4];
};

typedef struct IDE_Device IDE_Device;

void IDE_Init(void);

#endif

