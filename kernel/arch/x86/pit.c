#include "pit.h"

#include <sys/types.h>
#include <sys/limits.h>

#include "utils.h"

void PIT_Init()
{
  K_U16 divisor = (K_U16)(1193180 / K_TIMER_HERZ);
  PIO_Write8(0x43, 0x36);
  PIO_Write8(0x43, (K_U8)divisor);
  PIO_Write8(0x43, (K_U8)(divisor >> 8));
}

