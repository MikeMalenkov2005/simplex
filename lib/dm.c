#include <string.h>
#include <drv/dm.h>
#include <simplex.h>

int DM_Find(const char *name)
{
  DM_Packet packet = { 0 };
  packet.Command = DM_FIND;
  strncpy((char*)packet.String + 1, name, sizeof(packet.String) - 1);
  return sys_send(&packet, DM_TASK_ID) == -1 ? -1 : packet.Result;
}

int DM_Register(const char *name)
{
  DM_Packet packet = { 0 };
  packet.Command = DM_REGISTER;
  strncpy((char*)packet.String + 1, name, sizeof(packet.String) - 1);
  return sys_send(&packet, DM_TASK_ID) == -1 ? 0 : packet.Result;
}

