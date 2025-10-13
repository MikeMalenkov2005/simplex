#include "pit.h"

#include <sys/types.h>
#include <sys/limits.h>

#include "utils.h"

void PIT_Init()
{
  K_U16 divisor = (K_U16)(1193180 / K_TIMER_HERZ);
  K_WritePort8(0x43, 0x36);
  K_WritePort8(0x43, (K_U8)divisor);
  K_WritePort8(0x43, (K_U8)(divisor >> 8));
}

