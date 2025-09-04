#ifndef _X86_TASK_H
#define _X86_TASK_H

#include "isr.h"

struct TASK_Context
{
  K_U8 FPUState[512];
  ISR_Frame Frame;
};

typedef struct TASK_Context TASK_Context;

void TASK_SaveContext(ISR_Frame *frame);
void TASK_LoadContext(ISR_Frame *frame);

#endif

