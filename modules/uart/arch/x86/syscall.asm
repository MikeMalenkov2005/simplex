[bits 32]

extern UART_Main

global __entry
global __syscall

section .text

__entry:
  push eax
  call UART_Main
  mov ebx, eax
  xor eax, eax
  int 0x80

__syscall:
  push ebx
  mov eax, [esp + 8]
  mov ebx, [esp + 12]
  mov ecx, [esp + 16]
  mov edx, [esp + 20]
  int 0x80
  pop ebx
  ret

