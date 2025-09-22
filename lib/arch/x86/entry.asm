[bits 32]

global _start
global __crt_signal_handler

extern __crt_init
extern main
extern exit

extern raise

section .text

_start:
  call __crt_init
  call main
  call exit

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

