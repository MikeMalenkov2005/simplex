[bits 32]

global K_ZeroMemory
global K_WritePort8
global K_WritePort16
global K_WritePort32
global K_ReadPort8
global K_ReadPort16
global K_ReadPort32
global K_GetCPUFlags

global memcpy

section .text

K_ZeroMemory:
  push edi
  mov ecx, [esp + 12]
  mov edi, [esp + 8]
  mov edx, edi
  xor eax, eax
  rep stosb
  mov eax, edx
  pop edi
  ret

K_WritePort8:
  mov eax, [esp + 8]
  mov edx, [esp + 4]
  out dx, al
  ret

K_WritePort16:
  mov eax, [esp + 8]
  mov edx, [esp + 4]
  out dx, ax
  ret

K_WritePort32:
  mov eax, [esp + 8]
  mov edx, [esp + 4]
  out dx, eax
  ret

K_ReadPort8:
  mov edx, [esp + 4]
  xor eax, eax
  in al, dx
  ret

K_ReadPort16:
  mov edx, [esp + 4]
  xor eax, eax
  in ax, dx
  ret

K_ReadPort32:
  mov edx, [esp + 4]
  in eax, dx
  ret

K_GetCPUFlags:
  pushfd
  pop eax
  ret

