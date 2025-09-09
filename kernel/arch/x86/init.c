#include "boot.h"
#include "cpuid.h"
#include "idt.h"
#include "gdt.h"
#include "fpu.h"
#include "isr.h"
#include "mmu.h"
#include "task.h"

extern void __end;
extern void K_Stack;
static TSS K_MainTSS;

void K_Debug(K_U16 x, K_U16 y, K_U8 *string)
{
  K_U16 *screen = (K_HANDLE)0xB8000;
  K_U16 index = x + y * 80;
  while (*string) screen[index++] = 0x200 | *string++;
}

#define K_ToHex(c) (((c) & 15) < 10 ? ((c) & 15) + '0' : ((c) & 15) + 'A' - 10)

void K_DebugHex(K_U16 x, K_U16 y, K_U32 value)
{
  K_U8 string[9] = { 0 };
  K_U8 offset = sizeof string - 1;
  do { string[--offset] = K_ToHex(value); value >>= 4; } while (offset);
  K_Debug(x, y, string);
}

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
  K_Task *task;
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
  task = K_CreateTask(0, K_TASK_MODULE);
  K_SetTaskIP(task, K_MainInit);
  
  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MODULE)
  {
    /* TODO: Load modules as tasks */
  }

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_COMMAND_LINE)
  {
    K_SetTaskR0(task, (K_SSIZE)(K_USIZE)((K_BootTagCommandLine*)tag)->String);
    break;
  }
}

