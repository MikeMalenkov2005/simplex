#ifndef _X86_ISR_H
#define _X86_ISR_H

#include <sys/types.h>

struct ISR_Frame
{
  K_U32 Edi;
  K_U32 Esi;
  K_U32 Ebp;
  K_U32 Reserved;
  K_U32 Ebx;
  K_U32 Edx;
  K_U32 Ecx;
  K_U32 Eax;
  K_U32 Index;
  K_U32 Error;
  K_U32 Eip, Cs;
  K_U32 Eflags;
  K_U32 Esp, Ss;
};

typedef struct ISR_Frame ISR_Frame;

typedef void (*ISR_Handler)(ISR_Frame*);

void ISR_Init();

#endif

