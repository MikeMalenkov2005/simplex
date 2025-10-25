[bits 32]

global PIO_Write8
global PIO_Write16
global PIO_Write32

global PIO_Read8
global PIO_Read16
global PIO_Read32

section .text

PIO_Write8:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  out dx, al
  ret

PIO_Write16:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  out dx, ax
  ret

PIO_Write32:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  out dx, eax
  ret
           
PIO_Read8:
  mov edx, [esp + 4]
  xor eax, eax
  in al, dx
  ret

PIO_Read16:
  mov edx, [esp + 4]
  xor eax, eax
  in ax, dx
  ret

PIO_Read32:
  mov edx, [esp + 4]
  in eax, dx
  ret

