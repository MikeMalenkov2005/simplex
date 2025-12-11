[bits 32]

global __crt_atomic_swap
global __crt_atomic_get_add
global __crt_atomic_compare_swap

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

__crt_atomic_compare_swap:
  push ebx
  mov edx, [esp + 8]
  mov ebx, [esp + 12]
  mov ecx, [esp + 16]
  mov eax, [ebx]
  lock cmpxchg [edx], ecx
  setz cl
  mov [ebx], eax
  xor eax, eax
  mov al, cl
  pop ebx
  ret

