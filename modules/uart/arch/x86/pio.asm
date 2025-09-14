[bits 32]

global PIO_Read
global PIO_Write

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

PIO_Read:
  mov edx, [esp + 4]
  xor eax, eax
  in al, dx
  ret

PIO_Write:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  out dx, al
  ret

