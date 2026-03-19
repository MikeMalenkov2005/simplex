#include <drv/dm.h>
#include <drv/pci.h>
#include <simplex.h>

static int PCI = -1;

int PCI_Read(int device, int offset)
{
  PCI_Packet packet = { 0 };
  if (PCI == -1 && (PCI = DM_Wait("PCI", 128)) == -1) return -1;
  packet.Command = PCI_READ_CONFIG;
  packet.Offset = offset;
  packet.Device = device;
  packet.Config = -1;
  sys_send(&packet, PCI);
  return packet.Config;
}

int PCI_Find(int id, int type, int offset)
{
  PCI_Packet packet = { 0 };
  if (PCI == -1 && (PCI = DM_Wait("PCI", 128)) == -1) return -1;
  packet.Command = PCI_FIND_DEVICE;
  packet.Class = type >> 16;
  packet.Subclass = type >> 8;
  packet.Interface = type;
  packet.VendorID = id >> 16;
  packet.DeviceID = id;
  packet.Offset = offset;
  sys_send(&packet, PCI);
  return packet.Device;
}

