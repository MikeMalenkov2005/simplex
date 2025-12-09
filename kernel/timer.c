#include "timer.h"
#include "task.h"

K_USIZE K_Ticks;

void K_TickCallback()
{
  (void)K_SwitchTask();
  ++K_Ticks;
}

