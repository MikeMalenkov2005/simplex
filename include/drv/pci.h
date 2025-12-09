#ifndef _DRV_PCI_H
#define _DRV_PCI_H

#include <sys/types.h>

#define PCI_Device(bus, slot, function) \
  ((((bus) & 0xFF) << 8) | (((slot) & 0x1F) << 3) | ((function) & 7))

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

#endif

