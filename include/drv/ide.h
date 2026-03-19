#ifndef _DRV_IDE_H
#define _DRV_IDE_H

#include <stddef.h>
#include <sys/types.h>
#include <sys/limits.h>

#define IDE_GET_BUFFER      1
#define IDE_READ_SECTOR     2
#define IDE_WRITE_SECTOR    3
#define IDE_GET_SECTOR_SIZE 4
#define IDE_GET_DRIVE       5

#define IDE_GET_BUFFER_S      "\001"
#define IDE_READ_SECTOR_S     "\002"
#define IDE_WRITE_SECTOR_S    "\003"
#define IDE_GET_SECTOR_SIZE_S "\004"
#define IDE_GET_DRIVE_S       "\005"

struct IDE_Packet
{
  K_U8 Command;
  K_U8 Success;
  K_U16 Reserved;
  K_U32 Drive;
  K_U64 LBA;
  K_HANDLE Buffer;
  K_USIZE Size;
  K_U8 Padding[K_MESSAGE_SIZE - 16 - sizeof(K_HANDLE) - sizeof(K_USIZE)];
};

typedef struct IDE_Packet IDE_Packet;

/**
 * Allocates or gets a buffer for the calling task.
 * @return A pointer to the I/O buffer allocated for the calling task or NULL.
 */
void *IDE_GetBuffer(void);

/**
 * Reads a sector to the buffer allocated for the calling task.
 * @param lba The logical block address of the sector.
 * @param drive The drive handle created by this driver.
 * @return 0 on success or -1 on failure.
 */
int IDE_ReadSector(long long lba, int drive);

/**
 * Writes a sector from the buffer allocated for calling task.
 * @param lba The logical block address of the sector.
 * @param drive The drive handle created by this driver.
 * @return 0 on success or -1 on failure.
 */
int IDE_WriteSector(long long lba, int drive);

/**
 * Gets sector size of a drive.
 * @param drive A drive handle created by the driver.
 * @return The size of a sector for the drive or 0.
 */
size_t IDE_GetSectorSize(int drive);

/**
 * Gets a drive handle by index.
 * @param index The index of a drive.
 * @return A drive handle for a drive with specified index.
 */
int IDE_GetDrive(unsigned index);

#endif

