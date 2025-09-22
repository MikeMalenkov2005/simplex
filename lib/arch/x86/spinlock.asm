[bits 32]

global __crt_lock
global __crt_unlock

section .text

__crt_lock:
  mov eax, 1
  mov edx, [esp + 4]
  xchg [edx], eax
  test eax, eax
  jz .end
  mov eax, 10
  int 0x80
  jmp __crt_lock
.end:
  ret

__crt_unlock:
  xor eax, eax
  mov edx, [esp + 4]
  xchg [edx], eax
  ret

