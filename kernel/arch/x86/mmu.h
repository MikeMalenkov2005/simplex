#ifndef _X86_MMU_H
#define _X86_MMU_H

#include "../../memory.h"

#define MMU_PAGE_DIR    0xFFFFF000
#define MMU_PAGE_TABLE  0xFFC00000
#define MMU_INDEX_BITS  10
#define MMU_INDEX_MASK  0x3FF

#define MMU_PAGE_PRESENT        (1 << 0)
#define MMU_PAGE_WRITABLE       (1 << 1)
#define MMU_PAGE_READABLE       (1 << 10)
#define MMU_PAGE_EXECUTABLE     (1 << 11)
#define MMU_PAGE_USER_MODE      (1 << 2)
#define MMU_PAGE_GLOBAL         (1 << 8)
#define MMU_PAGE_EXTERNAL       (1 << 9)
#define MMU_PAGE_CACHE_DISABLE  (1 << 4)

extern K_USIZE K_FirstFreePage;
extern K_USIZE K_FreePageCount;

K_BOOL MMU_Init();
void MMU_Enable();

#endif

