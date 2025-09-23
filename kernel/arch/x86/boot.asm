[bits 32]

global K_Init
global K_Stack

global K_Idle
global K_IdleSize

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

BootAlignTag:
  dw 6
  dw 0
  dd $ - BootAlignTag

%endif

BootEndTag:
  dw 0
  dw 0
  dd $ - BootEndTag

BootHeader.end:

K_Init:
  cli
  cld
  cmp eax, LOADER_MAGIC
  jne .InvalidLoader
  mov esp, K_Stack
  mov ebp, esp
  sub esp, 60
  push esp
  push ebx
  call K_ArchInit
  add esp, 8
  mov eax, [esp+56]
  mov ds, ax
  mov es, ax
  popa
  add esp, 8
  iret
.InvalidLoader:
  jmp $

K_Idle:
  mov eax, 10
  int 0x80
  xor eax, eax
  jz K_Idle
.end:

section .data

K_IdleSize: dd K_Idle.end - K_Idle

section .bss

align 16
K_Stack.top:
resb 0x1000
K_Stack:

