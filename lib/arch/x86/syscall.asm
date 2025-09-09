[bits 32]

global syscall

section .text

syscall:
  push ebx
  mov eax, [esp + 8]
  cmp eax, 1
  je .L2
  cmp eax, 5
  jb .L1
  cmp eax, 9
  jb .L0
  je .L2
  cmp eax, 10
  je .L0
  cmp eax, 11
  je .L2
  cmp eax, 21
  je .L2
  cmp eax, 23
  jbe .L3
  cmp eax, 30
  je .L1
  cmp eax, 31
  je .L0
.L3:
  mov edx, [esp + 20]
.L2:
  mov ecx, [esp + 16]
.L1:
  mov ebx, [esp + 12]
.L0:
  int 0x80
  pop ebx
  ret

