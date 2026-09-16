#ifndef _K_ACPI_H
#define _K_ACPI_H

#include <sys/types.h>

struct K_RSDP
{
  K_U8  Signature[8];
  K_U8  Checksum;
  K_U8  OEMID[6];
  K_U8  Revision;
  K_U32 RSDTAddress;
};

typedef struct K_RSDP K_RSDP;

struct K_XSDP
{
  K_U8  Signature[8];
  K_U8  Checksum;
  K_U8  OEMID[6];
  K_U8  Revision;
  K_U32 RSDTAddress;
  K_U32 Length;
  K_U64 XSDTAddress;
  K_U8  ExtendedChecksum;
  K_U8  Reserved[3];
};

typedef struct K_XSDP K_XSDP;

struct K_SDTHeader
{
  K_U8  Signature[4];
  K_U32 Length;
  K_U8  Revision;
  K_U8  Checksum;
  K_U8  OEMID[6];
  K_U8  OEMTableID[8];
  K_U32 OEMRevision;
  K_U32 CreatorID;
  K_U32 CreatorRevision;
};

typedef struct K_SDTHeader K_SDTHeader;

struct K_RSDT
{
  K_SDTHeader Header;
  K_U32       Entries[0];
};

typedef struct K_RSDT K_RSDT;

struct K_XSDT
{
  K_SDTHeader Header;
  K_U64       Entries[0];
};

typedef struct K_XSDT K_XSDT;

struct K_FACP
{
  K_SDTHeader Header;
  /* TODO: Finish this structure! */
};

typedef struct K_FACP K_FACP;

#endif

