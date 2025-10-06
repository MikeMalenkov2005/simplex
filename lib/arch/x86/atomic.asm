[bits 32]

global __crt_atomic_swap

section .text

__crt_atomic_swap:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  xchg eax, [edx]
  ret

__crt_atomic_get_add:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  lock xadd [edx], eax
  ret

