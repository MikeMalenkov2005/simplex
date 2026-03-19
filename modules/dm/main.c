#include <string.h>
#include <simplex.h>
#include <drv/dm.h>

#define NAME_SIZE (K_MESSAGE_SIZE - 1)
#define MAX_TASKS 1024

static K_U8 names[MAX_TASKS][NAME_SIZE];
static int tasks[MAX_TASKS];

static int find(const K_U8 name[NAME_SIZE])
{
  int i, empty = MAX_TASKS;
  for (i = 0; i < MAX_TASKS; ++i)
  {
    if (tasks[i] != -1 && !memcmp(name, names[i], NAME_SIZE))
    {
      if (sys_check(tasks[i]) != -1) return i;
      tasks[i] = -1;
    }
    if (tasks[i] == -1 && i < empty) empty = i;
  }
  return -empty - 1;
}

int main(void)
{
  int tid = -1, i;
  DM_Packet packet;
  for (i = 0; i < MAX_TASKS; ++i) tasks[i] = -1;
  while ((tid = sys_wait(&packet)) != -1) 
  {
    switch (packet.Command)
    {
    case DM_FIND:
      i = find(packet.String + 1);
      if (i < 0) packet.Result = -1;
      else packet.Result = tasks[i];
      break;
    case DM_REGISTER:
      i = -find(packet.String + 1);
      if (i > 0 && --i < MAX_TASKS)
      {
        tasks[i] = tid;
        memcpy(names[i], packet.String + 1, NAME_SIZE);
        packet.Result = 1;
      }
      else packet.Result = 0;
      break;
    }
    sys_fire(&packet, tid);
  }
  return 0;
}

