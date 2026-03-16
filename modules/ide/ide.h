#ifndef _IDE_H
#define _IDE_H

#include <sys/types.h>

struct IDE
{
  K_U32 Device;
  K_HANDLE BAR[5];
};

typedef struct IDE IDE;

#define IDE_CHANNEL_PRIMARY   0
#define IDE_CHANNEL_SECONDARY 1

#define IDE_DRIVE_MASTER      0
#define IDE_DRIVE_SLAVE       1

#define IDE_TYPE_ATA          0
#define IDE_TYPE_ATAPI        1

struct IDE_Drive
{
  K_U32 CommandSets;
  K_U32 Sectors;
  K_U16 Features;
  K_U16 Signature;
  K_U8 Channel;
  K_U8 Drive;
  K_U8 Type;
  K_U8 Model[41];
};

typedef struct IDE_Drive IDE_Drive;

extern int PCI;

void IDE_Init(K_U32 device);

K_HANDLE IDE_MapBAR(K_U32 device, K_U8 index);
K_U8 IDE_ReadBAR(K_HANDLE base, K_U8 offset);
void IDE_WriteBAR(K_HANDLE base, K_U8 offset, K_U8 byte);

#endif

