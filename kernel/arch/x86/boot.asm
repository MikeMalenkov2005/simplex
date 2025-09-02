[bits 32]

global K_Init
global K_Stack

extern K_Main
extern K_ArchInit
extern __boot_start
extern __bss_start
extern __end

KERNEL_MAGIC equ 0xE85250D6
LOADER_MAGIC equ 0x36d76289

section .boot

BootHeader:
  dd KERNEL_MAGIC
  dd 0
  dd .end - BootHeader
  dd BootHeader - KERNEL_MAGIC - .end

BootInfoTag:
  dw 1
  dw 0
  dd .end - BootInfoTag
  dd 1, 3, 4, 5, 6
.end:

%ifndef K_ELF

BootLoadTag:
  dw 2
  dw 0
  dd .end - BootLoadTag
  dd BootHeader
  dd __boot_start
  dd __bss_start
  dd __end
.end:

BootEntryTag:
  dw 3
  dw 0
  dd .end - BootEntryTag
  dd K_Init
.end:

%endif

BootEndTag:
  dw 0
  dw 0
  dd $ - BootEndTag

BootHeader.end:

K_Init:
  cmp eax, LOADER_MAGIC
  jne .InvalidLoader
  mov esp, K_Stack
  mov ebp, esp
  push ebx
  call K_ArchInit
  mov [esp], eax
  call K_Main
.InvalidLoader:
  jmp $

section .bss

align 16
K_Stack.top:
resb 0x1000
K_Stack:

