#include "boot.h"
#include "cpuid.h"
#include "idt.h"
#include "gdt.h"
#include "fpu.h"
#include "isr.h"
#include "mmu.h"
#include "task.h"
#include "utils.h"

extern void K_Idle;
extern K_USIZE K_IdleSize;

extern void __end;
extern void K_Stack;
static TSS K_MainTSS;

extern void K_Panic(const char *message);

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

void K_ArchInit(K_BootInfo *info, ISR_Frame *frame)
{
  K_BootTag *tag;
  K_BootTagMemoryMapEntry *ent;
  K_Task *idle, module;
  K_HANDLE address;
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

  if (!K_FreePageCount) K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MEMORY_INFO)
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
  K_InitTaskSlots();
  
  TASK_SetFrame(frame);
  idle = K_CreateTask(0, K_TASK_MODULE);
  address = K_FindFirstFreeAddress(K_IdleSize);
  if (!address || !K_AllocatePage(address, K_PAGE_READABLE | K_PAGE_EXECUTABLE | K_PAGE_USER_MODE)) K_Panic("no idle!");
  K_SetTaskIP(idle, memcpy(address, &K_Idle, K_IdleSize));
  
  (void)module;
  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MODULE)
  {
    /* TODO: Load modules as tasks */
  }

  K_SetPageMap(idle->PageMap);

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_COMMAND_LINE)
  {
    /* TODO: Handle command line */
  }
}

