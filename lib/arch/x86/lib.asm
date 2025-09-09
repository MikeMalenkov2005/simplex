[bits 32]

global memcmp
global memcpy
global memset

section .text

memcmp:
  push esi
  push edi
  mov edi, [esp + 12]
  mov esi, [esp + 16]
  mov ecx, [esp + 20]
  xor eax, eax
  mov edx, eax
  repe cmpsb
  setge al
  setle dl
  sub eax, edx
  pop edi
  pop esi
  ret

memcpy:
  push esi
  push edi
  mov edi, [esp + 12]
  mov esi, [esp + 16]
  mov ecx, [esp + 20]
  mov edx, edi
  rep movsb
  mov eax, edx
  pop edi
  pop esi
  ret

memset:
  push edi
  mov edi, [esp + 8]
  mov eax, [esp + 12]
  mov ecx, [esp + 16]
  mov edx, edi
  rep stosb
  mov eax, edx
  pop edi
  ret

