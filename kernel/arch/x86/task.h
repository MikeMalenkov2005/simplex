#ifndef _X86_TASK_H
#define _X86_TASK_H

#include "isr.h"
#include "../../task.h"

struct TASK_Context
{
  K_HANDLE pStackTop;
  K_USIZE StackSize;
  ISR_Frame Frame;
};

typedef struct TASK_Context TASK_Context;

void TASK_SetFrame(ISR_Frame *frame);

#endif

