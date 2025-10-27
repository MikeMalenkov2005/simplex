#ifndef _K_TIMER_H
#define _K_TIMER_H

#include <sys/types.h>

extern K_U32 K_Ticks;

void K_TickCallback();

K_SSIZE K_GetRealTime();

K_BOOL K_SetRealTime(K_SSIZE value);

#endif

