[bits 32]

global MMU_Enable

global K_GetPageMap
global K_SetPageMap
global K_InvalidatePage

extern K_FirstFreePage
extern K_FreePageCount

extern __end

section .text

MMU_Enable:
  mov eax, cr0
  or eax, 0x80000001
  mov cr0, eax
  ret

K_GetPageMap:
  mov eax, cr3
  ret

K_SetPageMap:
  mov eax, [esp + 4]
  mov cr3, eax
  ret

K_InvalidatePage:
  mov eax, [esp + 4]
  and eax, 0xFFFFF000
  invlpg [eax]
  ret

