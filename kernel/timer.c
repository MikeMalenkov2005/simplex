#include "timer.h"
#include "task.h"

K_U32 K_Ticks;

void K_TickCallback()
{
  (void)K_SwitchTask();
  ++K_Ticks;
}

