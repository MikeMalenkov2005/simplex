[bits 32]

global CPUID_Init
global CPUID_Execute

CPUID_FLAG  equ 0x00200000

section .text

CPUID_Init:
  pushfd
  pushfd
  xor dword [esp], CPUID_FLAG
  popfd
  pushfd
  pop eax
  xor eax, [esp]
  popfd
  and eax, CPUID_FLAG
  jz .Exit
  xor eax, eax
  push ebx
  cpuid
  inc eax
  pop ebx
.Exit:
  mov [CPUID_Count], eax
  ret

CPUID_Execute:
  push ebx
  push edi
  mov eax, [esp + 12]
  mov edi, [esp + 16]
  cmp eax, [CPUID_Count]
  jb .Valid
  xor eax, eax
  mov ecx, eax
  mov edx, eax
  mov ebx, eax
  jmp .Exit
.Valid:
  cpuid
.Exit:
  mov [edi], ebx
  mov [edi + 4], edx
  mov [edi + 8], ecx
  mov [edi + 12], eax
  pop edi
  pop ebx
  ret

section .bss

CPUID_Count:  resd 1

