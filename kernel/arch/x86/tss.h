#ifndef _X86_TSS_H
#define _X86_TSS_H

#include <sys/types.h>

struct TSS
{
  K_U32 Link;
  K_U32 Esp0;
  K_U32 Ss0;
  K_U32 Esp1;
  K_U32 Ss1;
  K_U32 Esp2;
  K_U32 Ss2;
  K_U32 Cr3;
  K_U32 Eip;
  K_U32 Eflags;
  K_U32 Eax;
  K_U32 Ecx;
  K_U32 Edx;
  K_U32 Ebx;
  K_U32 Esp;
  K_U32 Ebp;
  K_U32 Esi;
  K_U32 Edi;
  K_U32 Es;
  K_U32 Cs;
  K_U32 Ss;
  K_U32 Ds;
  K_U32 Fs;
  K_U32 Gs;
  K_U32 Ldtr;
  K_U16 TFlag;
  K_U16 IOPBOffset;
  K_U32 Ssp;
};

typedef struct TSS TSS;

void TSS_Init(TSS *tss, void *esp0);

#endif

