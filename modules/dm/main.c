#include <string.h>
#include <simplex.h>
#include <drv/dm.h>

#define NAME_SIZE (K_MESSAGE_SIZE - 1)
#define MAX_TASKS 1024

static K_U8 names[MAX_TASKS][NAME_SIZE];
static int tasks[MAX_TASKS];

#include <drv/uart.h>

static int uart = -1;

static void logch(int ch)
{
  UART_Packet packet = { 0 };
  packet.Command = UART_TRANSMIT;
  packet.Data.Size = 1;
  packet.Data.Buffer[0] = ch;
  sys_send(&packet, uart);
}

static void logu8(K_U8 u8)
{
  logch(((u8 >> 4) & 15) + (((u8 >> 4) & 15) < 10 ? '0' : 'A' - 10));
  logch(((u8 >> 0) & 15) + (((u8 >> 0) & 15) < 10 ? '0' : 'A' - 10));
}

static void logu16(K_U16 u16)
{
  logu8(u16 >> 8);
  logu8(u16 >> 0);
}

static void logu32(K_U32 u32)
{
  logu16(u32 >> 16);
  logu16(u32 >>  0);
}

static void logname(const K_U8 name[NAME_SIZE])
{
  int i;
  for (i = 0; i < NAME_SIZE; ++i) logch(name[i]);
}

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
        if (uart != -1)
        {
          logu32(tid);
          logch(' ');
          logname(names[i]);
          logch('\r');
          logch('\n');
        }
        else if (!strncmp((char*)packet.String + 1, "UART", NAME_SIZE)) uart = tid;
        packet.Result = 1;
      }
      else packet.Result = 0;
      break;
    }
    sys_fire(&packet, tid);
  }
  return 0;
}

