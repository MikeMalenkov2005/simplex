#ifndef _DRV_PCI_H
#define _DRV_PCI_H

#include <sys/types.h>
#include <sys/limits.h>

#define PCI_READ_CONFIG 1
#define PCI_FIND_DEVICE 3

#define PCI_READ_CONFIG_S "\001"
#define PCI_FIND_DEVICE_S "\003"

#define PCI_Device(bus, slot, function) \
  ((((bus) & 0xFF) << 8) | (((slot) & 0x1F) << 3) | ((function) & 7))

#define PCI_Id(vendor, device) \
  ((((vendor) & 0xFFFFU) << 16) | ((device) & 0xFFFFU))

#define PCI_Type(class, subclass, interface) \
  ((((class) & 0xFF) << 16) | (((subclass) & 0xFF) << 8) | ((interface) & 0xFF))

#define PCI_INVALID_DEVICE  (~(K_U32)0)

#define PCI_OFFSET_VENDOR_ID        0
#define PCI_OFFSET_DEVICE_ID        2
#define PCI_OFFSET_COMMAND          4
#define PCI_OFFSET_STATUS           6
#define PCI_OFFSET_REVISION         8
#define PCI_OFFSET_INTERFACE        9
#define PCI_OFFSET_SUBCLASS         10
#define PCI_OFFSET_CLASS            11
#define PCI_OFFSET_CHACHE_LINE_SIZE 12
#define PCI_OFFSET_LATENCY_TIMER    13
#define PCI_OFFSET_HEADER_TYPE      14
#define PCI_OFFSET_BIST             15

#define PCI_OFFSET_BAR0 16
#define PCI_OFFSET_BAR1 20
#define PCI_OFFSET_BAR2 24
#define PCI_OFFSET_BAR3 28
#define PCI_OFFSET_BAR4 32
#define PCI_OFFSET_BAR5 36
#define PCI_OFFSET_CIS  40

struct PCI_Packet
{
  K_U8 Command;
  
  K_U8 Class;
  K_U8 Subclass;
  K_U8 Interface;
  K_U16 VendorID;
  K_U16 DeviceID;
  
  K_U32 Offset;
  K_U32 Device;
  K_U32 Config;
  
  K_U8 Padding[K_MESSAGE_SIZE - 20];
};

typedef struct PCI_Packet PCI_Packet;

/**
 * @param device The PCI device address (returned by PCI_Find).
 * @param offset The offset into the configuration space.
 * @return shifted config register on success or -1 on failure.
 */
int PCI_Read(int device, int offset);

/**
 * @param id The full device id ((VendorID << 16)|DeviceID).
 * @param type The full device type ((Class << 16)|(Subclass << 8)|Interface).
 * @param offset The offset into the list of found devices.
 * @return The PCI device address on success or -1 on failure.
 */
int PCI_Find(int id, int type, int offset);

#endif

