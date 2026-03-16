#include "ide.h"

#include <drv/dm.h>
#include <drv/pci.h>
#include <simplex.h>

int PCI;

static void init(void)
{
  K_U32 offset = 0;
  PCI_Packet packet = { 0 };
  packet.Command = PCI_FIND_DEVICE;
  packet.Class = 1;
  packet.Subclass = 1;
  packet.Interface = -1;
  packet.VendorID = -1;
  packet.DeviceID = -1;
  while (sys_send(&packet, PCI) != -1 && ~packet.Device)
  {
    IDE_Init(packet.Device);
    packet.Offset = ++offset;
  }
}

int main(void)
{
  if (!DM_Register("IDE")) return 1;
  if ((PCI = DM_Wait("PCI", 512)) == -1) return 1;
  init();
  return 0;
}

