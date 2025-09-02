#include "boot.h"
#include "gdt.h"
#include "idt.h"
#include "mmu.h"

extern void K_Stack;
static TSS K_MainTSS;

K_U8 *K_ArchInit(K_BootInfo *info)
{
  K_BootTag *tag;
  K_USIZE lower, upper;
  K_U8 *pCmdLine = NULL;

  IDT_Init();
  TSS_Init(&K_MainTSS, &K_Stack);
  GDT_Init(&K_MainTSS);

  K_BootForEach(info, tag) switch (tag->Type)
  {
  case K_BOOT_TAG_COMMAND_LINE:
    pCmdLine = ((K_BootTagCommandLine*)tag)->String;
    break;
  case K_BOOT_TAG_MEMORY_INFO:
    lower = (K_USIZE)((K_BootTagMemoryInfo*)tag)->Lower << 10;
    upper = (K_USIZE)((K_BootTagMemoryInfo*)tag)->Upper << 10;
    break;
  case K_BOOT_TAG_BIOS_DEVICE:
    break;
  }

  MMU_Init(lower, upper);

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MEMORY_MAP)
  {
  }

  K_BootForEach(info, tag) if (tag->Type == K_BOOT_TAG_MODULE)
  {
  }

  return pCmdLine;
}

