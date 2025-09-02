#ifndef _X86_GDT_H
#define _X86_GDT_H

#include "tss.h"

struct GDT
{
  K_U32 Null[2];
  K_U32 KernelCode[2];
  K_U32 KernelData[2];
  K_U32 UserCode[2];
  K_U32 UserData[2];
  K_U32 TastState[2];
};

typedef struct GDT GDT;

void GDT_Init(TSS *tss);

void GDT_Create(GDT *gdt, TSS *tss);

#endif

