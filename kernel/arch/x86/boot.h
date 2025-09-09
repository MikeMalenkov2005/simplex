#ifndef _X86_BOOT_H
#define _X86_BOOT_H

#include <types.h>

struct K_BootInfo
{
  K_U32 Size;
  K_U32 Reserved;
};

typedef struct K_BootInfo K_BootInfo;

struct K_BootTag
{
  K_U32 Type;
  K_U32 Size;
};

typedef struct K_BootTag K_BootTag;

#define K_BootFirstTag(inf)     ((K_BootTag*)((inf) + 1))
#define K_BootNextTag(tag)      ((K_BootTag*)(((K_USIZE)(tag) + (tag)->Size + 7) & (~(K_USIZE)0 << 3)))
#define K_BootForEach(inf, tag) for ((tag) = K_BootFirstTag(inf); (tag)->Type; (tag) = K_BootNextTag(tag))

#define K_BOOT_TAG_COMMAND_LINE 1
#define K_BOOT_TAG_LOADER_NAME  2
#define K_BOOT_TAG_MODULE       3
#define K_BOOT_TAG_MEMORY_INFO  4
#define K_BOOT_TAG_BIOS_DEVICE  5
#define K_BOOT_TAG_MEMORY_MAP   6

struct K_BootTagCommandLine
{
  K_BootTag Tag;
  K_U8 String[0];
};

typedef struct K_BootTagCommandLine K_BootTagCommandLine;

struct K_BootTagLoaderName
{
  K_BootTag Tag;
  K_U8 String[0];
};

typedef struct K_BootTagLoaderName K_BootTagLoaderName;

struct K_BootTagModule
{
  K_BootTag Tag;
  K_U32 Start;
  K_U32 End;
  K_U8 String[0];
};

typedef struct K_BootTagModule K_BootTagModule;

struct K_BootTagMemoryInfo
{
  K_BootTag Tag;
  K_U32 Lower;
  K_U32 Upper;
};

typedef struct K_BootTagMemoryInfo K_BootTagMemoryInfo;

struct K_BootTagBIOSDevice
{
  K_BootTag Tag;
  K_U32 DeviceNumber;
  K_U32 Partition;
  K_U32 SubPartition;
};

typedef struct K_BootTagBIOSDevice K_BootTagBIOSDevice;

struct K_BootTagMemoryMap
{
  K_BootTag Tag;
  K_U32 EntrySize;
  K_U32 EntryVersion;
};

typedef struct K_BootTagMemoryMap K_BootTagMemoryMap;

struct K_BootTagMemoryMapEntry
{
  K_U32 BaseLow;
  K_U32 BaseHigh;
  K_U32 LengthLow;
  K_U32 LengthHigh;
  K_U32 Type;
  K_U32 Reserved;
};

typedef struct K_BootTagMemoryMapEntry K_BootTagMemoryMapEntry;

#define K_BootTagMemoryMapFirstEntry(map)     ((K_BootTagMemoryMapEntry*)((map) + 1))
#define K_BootTagMemoryMapNextEntry(map, ent) ((K_BootTagMemoryMapEntry*)((K_U8*)(ent) + (map)->EntrySize))
#define K_BootTagMemoryMapIsValid(map, ent)   ((K_USIZE)(ent) - (K_USIZE)(map) < (map)->Tag.Size)
#define K_BootTagMemoryMapForEach(map, ent) \
  for ((ent) = K_BootTagMemoryMapFirstEntry(map); K_BootTagMemoryMapIsValid(map, ent); (ent) = K_BootTagMemoryMapNextEntry(map, ent))

#define K_BOOT_TAG_MEMORY_MAP_FREE      1
#define K_BOOT_TAG_MEMORY_MAP_ACPI      3
#define K_BOOT_TAG_MEMORY_MAP_PRESERVED 4
#define K_BOOT_TAG_MEMORY_MAP_DEFECTIVE 5

#endif

