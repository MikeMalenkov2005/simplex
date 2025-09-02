[bits 32]

global GDT_Init
global GDT_Create

section .text

GDT_Init:
  mov eax, [esp + 4]
  mov [GDT.tss + 2], ax
  shr eax, 16
  mov [GDT.tss + 4], al
  mov [GDT.tss + 7], ah
  lgdt [GDTR]
  jmp 0x08:.LoadCS
.LoadCS:
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov ax, 0x28
  ltr ax
  ret

GDT_Create:
  push edi
  push esi
  mov edi, [esp + 12]
  mov esi, GDT
  mov ecx, GDT - GDT.end
  rep movsb
  mov eax, [esp + 16]
  mov [edi - 6], ax
  shr eax, 16
  mov [edi - 4], al
  mov [edi - 1], ah
  pop esi
  pop edi
  ret

section .data

align 16

GDT:
.null:
  dq 0
.kcode:
  dw 0xFFFF, 0
  db 0, 0x9A, 0xCF, 0
.kdata:
  dw 0xFFFF, 0
  db 0, 0x92, 0xCF, 0
.ucode:
  dw 0xFFFF, 0
  db 0, 0xFA, 0xCF, 0
.udata:
  dw 0xFFFF, 0
  db 0, 0xF2, 0xCF, 0
.tss:
  dw 107, 0
  db 0, 0x89, 0, 0
.end:

GDTR:
  dw GDT.end - GDT - 1
  dd GDT

