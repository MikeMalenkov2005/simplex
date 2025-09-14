[bits 32]

global _start

extern main
extern raise

global __crt_heap_base
global __crt_heap_size

section .text

_start:
  mov eax, 12
  lea ebx, [__crt_signal_handler]
  int 0x80
  call main
  mov ebx, eax
  xor eax, eax
  int 0x80

__crt_signal_handler:
  xchg eax, [esp + 4]
  push ecx ; save volatile registers
  push edx
  push eax ; pass signal number to raise
  call raise
  add esp, 4
  pop edx
  pop ecx ; restore volatile registers
  xchg eax, [esp + 4]
  ret 4 ; return and discard signal number

section .data

__crt_heap_base: dd 0
__crt_heap_size: dd 0

