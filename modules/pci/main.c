#include "pci.h"

#include <sys/limits.h>
#include <simplex.h>

int main(void)
{
  K_U8 message[K_MESSAGE_SIZE];
  int tid;
  PCI_ScanDevices();
  while ((tid = sys_wait(message)) != -1)
  {
  }
  return 0;
}

