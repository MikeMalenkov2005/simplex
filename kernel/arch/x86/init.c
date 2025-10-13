#include "boot.h"
#include "cpuid.h"
#include "idt.h"
#include "gdt.h"
#include "fpu.h"
#include "isr.h"
#include "mmu.h"
#include "pit.h"
#include "task.h"
#include "../../utils.h"
#include "../../loader.h"

extern void K_Idle;
extern K_USIZE K_IdleSize;

extern void __end;
extern void K_Stack;
static TSS K_MainTSS;

extern void K_DebugHex(K_U16 x, K_U16 y, K_U32 value);
extern void K_Debug(K_U16 x, K_U16 y, const char *string);
extern void K_Panic(const char *message);

static K_U32 failed = 0;

K_BOOL K_IsPageFree(K_USIZE page, K_BootInfo *info)
{
  K_BootTag *tag;
  page = K_PageDown(page);
  if (page >= K_PageDown(info) && page < K_PageUp((K_USIZE)info + info->Size)) return FALSE;
  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MODULE)
  {
    if (page >= ((K_BootTagModule*)tag)->Start && page < ((K_BootTagModule*)tag)->End) return FALSE;
  }
  return TRUE;
}

K_BOOL K_MapInfo(volatile K_BootInfo *info)
{
  K_USIZE limit;
  K_USIZE page = K_PageDown(info) | K_PAGE_VALID | K_PAGE_USER_MODE | K_PAGE_READABLE | K_PAGE_GLOBAL;
  if (!K_SetPage((K_HANDLE)page, page)) return FALSE;
  limit = K_PageUp((K_USIZE)info + info->Size);
  for (page += K_PAGE_SIZE; page < limit; page += K_PAGE_SIZE)
  {
    if (!K_SetPage((K_HANDLE)page, page)) return FALSE;
  }
  return TRUE;
}

K_HANDLE K_MapModule(const K_BootTagModule *mod)
{
  K_HANDLE result = K_FindLastFreeAddress(mod->End - mod->Start);
  K_USIZE size = mod->End - mod->Start;
  K_USIZE offset;
  if (result) for (offset = 0; offset < size; offset += K_PAGE_SIZE)
  {
    if (!K_SetPage(result + offset, mod->Start + offset | K_PAGE_VALID | K_PAGE_READABLE))
    {
      size = offset;
      for (offset = 0; offset < size; offset += K_PAGE_SIZE)
      {
        (void)K_FreePage(result + offset);
      }
      result = NULL;
    }
  }
  return result;
}

void K_ArchInit(K_BootInfo *info, ISR_Frame *frame)
{
  K_BootTag *tag;
  K_BootTagModule *mod;
  K_BootTagMemoryMapEntry *ent;
  K_Task *idle, *module;
  K_HANDLE address, entry;
  K_USIZE page;

  IDT_Init();
  TSS_Init(&K_MainTSS, &K_Stack);
  GDT_Init(&K_MainTSS);
  
  PIT_Init();
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

  if (!MMU_Init()) K_Panic("no mmu!");
  if (!K_MapInfo(info)) K_Panic("no info!");

  K_InitTaskSlots();
  TASK_SetFrame(frame);
  idle = K_CreateTask(0, K_TASK_MODULE);
  address = K_FindFirstFreeAddress(K_IdleSize);
  if (!address || !K_AllocatePage(address, K_PAGE_READABLE | K_PAGE_EXECUTABLE | K_PAGE_USER_MODE)) K_Panic("no idle!");
  K_SetTaskIP(idle, memcpy(address, &K_Idle, K_IdleSize));

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MODULE)
  {
    mod = (K_HANDLE)tag;
    module = K_CreateTask(0, K_TASK_MODULE);
    if (module)
    {
      K_SetPageMap(module->PageMap);
      address = K_MapModule(mod);
      if (!address || !K_LoadTaskImage(address, mod->End - mod->Start, &entry))
      {
        K_DebugHex(0, failed++, mod->Start);
        K_SetPageMap(idle->PageMap);
        K_DeleteTask(module);
      }
      else
      {
        K_FreePages(address, mod->End - mod->Start);
        K_SetPageMap(idle->PageMap);
        K_SetTaskIP(module, entry);
        K_SetTaskR0(module, (K_USIZE)mod->String);
      }
    }
  }

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_COMMAND_LINE)
  {
    K_SetTaskR0(idle, (K_USIZE)(tag + 1));
  }
}

