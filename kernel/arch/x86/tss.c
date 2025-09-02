#include "tss.h"

#include "utils.h"

void TSS_Init(TSS *tss, void *esp0)
{
  tss = K_ZeroStruct(tss);
  tss->Ss0 = 0x10;
  tss->Esp0 = (K_U32)(K_USIZE)esp0;
}

