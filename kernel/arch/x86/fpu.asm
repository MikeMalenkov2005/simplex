[bits 32]

global FPU_Init
global FPU_Save
global FPU_Load

global FPU_Mode
global FPU_Size

extern CPUID_Execute

section .text

; TODO: Add XSAVE/XRSTOR and other extensions support

FPU_Init:
  mov eax, cr0
  and al, 0xF3
  or al, 2
  mov cr0, eax
  mov eax, 0x5A5A
  fninit
  fnstsw ax ; TODO: Check if this actually works on 486SX with no FPU.
  test eax, eax
  jnz .NoFPU
  sub esp, 16
  push esp
  push 1
  call CPUID_Execute
  add esp, 24
  xor eax, eax
  test edx, 0x02000000
  jz .NoSSE
  mov al, 2
  mov [FPU_Mode], eax
  shl eax, 8
  mov [FPU_Size], eax
  mov eax, cr4
  or ah, 6
  mov cr4, eax
  ret
.NoSSE:
  mov al, 1
  mov [FPU_Mode], eax
  mov al, 108
  mov [FPU_Size], eax
  ret
.NoFPU:
  mov eax, cr0
  and al, 0xFD
  or al, 4
  mov cr0, eax
  xor eax, eax
  mov [FPU_Mode], eax
  mov [FPU_Size], eax
  ret

FPU_Save:
  mov eax, [FPU_Mode]
  test eax, eax
  jz .NoFPU
  mov ecx, [esp + 4]
  dec eax
  jz .NoSSE
  fxsave [ecx]
.NoFPU:
  ret
.NoSSE:
  fnsave [ecx]
  ret

FPU_Load:
  mov eax, [FPU_Mode]
  test eax, eax
  jz .NoFPU
  mov ecx, [esp + 4]
  dec eax
  jz .NoSSE
  fxrstor [ecx]
.NoFPU:
  ret
.NoSSE:
  frstor [ecx]
  ret

section .bss

FPU_Mode: resd 1
FPU_Size: resd 1

