#include <drv/dm.h>
#include <drv/ide.h>
#include <simplex.h>

static int IDE = -1;

void *IDE_GetBuffer(void)
{
  IDE_Packet packet = { 0 };
  if (IDE == -1 && (IDE = DM_Wait("IDE", 128)) == -1) return NULL;
  packet.Command = IDE_GET_BUFFER;
  return packet.Buffer;
}

int IDE_ReadSector(long long lba, int drive)
{
  IDE_Packet packet = { 0 };
  if (IDE == -1 && (IDE = DM_Wait("IDE", 128)) == -1) return -1;
  packet.Command = IDE_READ_SECTOR;
  packet.Drive = drive;
  packet.LBA = lba;
  return -!packet.Success;
}

int IDE_WriteSector(long long lba, int drive)
{
  IDE_Packet packet = { 0 };
  if (IDE == -1 && (IDE = DM_Wait("IDE", 128)) == -1) return -1;
  packet.Command = IDE_WRITE_SECTOR;
  packet.Drive = drive;
  packet.LBA = lba;
  return -!packet.Success;
}

size_t IDE_GetSectorSize(int drive)
{
  IDE_Packet packet = { 0 };
  if (IDE == -1 && (IDE = DM_Wait("IDE", 128)) == -1) return -1;
  packet.Command = IDE_READ_SECTOR;
  packet.Drive = drive;
  return packet.Size;
}

int IDE_GetDrive(unsigned index)
{
  IDE_Packet packet = { 0 };
  if (IDE == -1 && (IDE = DM_Wait("IDE", 128)) == -1) return -1;
  packet.Command = IDE_READ_SECTOR;
  packet.Drive = index;
  return packet.Success ? packet.Drive : -1;
}

