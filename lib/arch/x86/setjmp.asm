[bits 32]

global setjmp
global longjmp

JB_BP equ 0
JB_BX equ 4
JB_DI equ 8
JB_SI equ 12
JB_SP equ 16
JB_PC equ 20

section .text

setjmp:
  mov edx, [esp + 4]
  mov [edx+JB_BP], ebp
  mov [edx+JB_BX], ebx
  mov [edx+JB_DI], edi
  mov [edx+JB_SI], esi
  lea eax, [esp + 4]
  mov [edx+JB_SP], eax
  mov eax, [esp]
  mov [edx+JB_PC], eax
  xor eax, eax
  ret

longjmp:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  mov ebp, [edx+JB_BP]
  mov ebx, [edx+JB_BX]
  mov edi, [edx+JB_DI]
  mov esi, [edx+JB_SI]
  mov esp, [edx+JB_SP]
  mov ecx, [edx+JB_PC]
  jmp ecx

