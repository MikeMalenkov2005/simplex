[bits 32]

global __syscall

section .text

__syscall:
  push ebx
  mov eax, [esp + 8]
  mov ebx, [esp + 12]
  mov ecx, [esp + 16]
  mov edx, [esp + 20]
  int 0x80
  pop ebx
  ret

