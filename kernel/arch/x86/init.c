#include "boot.h"
#include "cpuid.h"
#include "idt.h"
#include "gdt.h"
#include "fpu.h"
#include "mmu.h"

extern void __end;
extern void K_Stack;
static TSS K_MainTSS;

K_BOOL K_IsPageFree(K_USIZE page, K_BootInfo *info)
{
  K_BootTag *tag;
  page = K_PageDown(page);
  if (page >= K_PageDown(info) && page < K_PageUp((K_USIZE)info + info->Size)) return FALSE;
  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MODULE)
  {
    if (page >= K_PageDown(((K_BootTagModule*)tag)->Start) && page < K_PageUp(((K_BootTagModule*)tag)->End)) return FALSE;
  }
  return TRUE;
}

K_U8 *K_ArchInit(K_BootInfo *info)
{
  K_BootTag *tag;
  K_BootTagMemoryMapEntry *ent;
  K_USIZE page;

  IDT_Init();
  TSS_Init(&K_MainTSS, &K_Stack);
  GDT_Init(&K_MainTSS);
  
  CPUID_Init();
  FPU_Init();

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MEMORY_MAP) 
  {
    K_BootTagMemoryMapForEach((K_BootTagMemoryMap*)tag, ent)
    {
      /* TODO: Parse Memory Map */
    }
  }

  if (!K_FirstFreePage) K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MEMORY_INFO)
  {
    page = K_PageDown((K_USIZE)((K_BootTagMemoryInfo*)tag)->Upper << 10) + 0x100000;
    while (page > K_PageUp(&__end))
    {
      page -= K_PAGE_SIZE;
      if (K_IsPageFree(page, info))
      {
        *(K_USIZE*)page = K_FirstFreePage;
        K_FirstFreePage = page;
        ++K_FreePageCount;
      }
    }
  }

  MMU_Init();

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MODULE)
  {
  }

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_COMMAND_LINE)
  {
    return ((K_BootTagCommandLine*)tag)->String;
  }
  return NULL;
}

