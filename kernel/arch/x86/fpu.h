#ifndef _X86_FPU_H
#define _X86_FPU_H

#include <types.h>

void FPU_Init();
void FPU_Save(K_HANDLE fpu);
void FPU_Load(K_HANDLE fpu);

extern const K_U32 FPU_Mode;
extern const K_U32 FPU_Size;

#define FPU_MODE_NONE 0
#define FPU_MODE_FPU  1
#define FPU_MODE_SSE  2

#endif

