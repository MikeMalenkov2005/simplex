[bits 32]

extern IDT_Dispatch

global ISR_0
global ISR_1
global ISR_2
global ISR_3
global ISR_4
global ISR_5
global ISR_6
global ISR_7
global ISR_8
global ISR_9
global ISR_10
global ISR_11
global ISR_12
global ISR_13
global ISR_14
global ISR_15
global ISR_16
global ISR_17
global ISR_18
global ISR_19
global ISR_20
global ISR_21
global ISR_22
global ISR_23
global ISR_24
global ISR_25
global ISR_26
global ISR_27
global ISR_28
global ISR_29
global ISR_30
global ISR_31
global ISR_32
global ISR_33
global ISR_34
global ISR_35
global ISR_36
global ISR_37
global ISR_38
global ISR_39
global ISR_40
global ISR_41
global ISR_42
global ISR_43
global ISR_44
global ISR_45
global ISR_46
global ISR_47
global ISR_128

global IDT_Load

section .text

IDT_Load:
  lidt [IDTR]
  ret

ISR_Common:
  pusha
  mov ax, ss
  mov ds, ax
  mov es, ax
  call IDT_Dispatch
  mov eax, [esp+56]
  mov ds, ax
  mov es, ax
  popa
  iret

%macro ISR_WithError 1
ISR_%1:
  push %1
  jmp ISR_Common
%endmacro

%macro ISR_NoError 1
ISR_%1:
  push 0
  push %1
  jmp ISR_Common
%endmacro

ISR_NoError 0
ISR_NoError 1
ISR_NoError 2
ISR_NoError 3
ISR_NoError 4
ISR_NoError 5
ISR_NoError 6
ISR_NoError 7
ISR_WithError 8
ISR_NoError 9
ISR_WithError 10
ISR_WithError 11
ISR_WithError 12
ISR_WithError 13
ISR_WithError 14
ISR_NoError 15
ISR_NoError 16
ISR_WithError 17
ISR_NoError 18
ISR_NoError 19
ISR_NoError 20
ISR_WithError 21
ISR_NoError 22
ISR_NoError 23
ISR_NoError 24
ISR_NoError 25
ISR_NoError 26
ISR_NoError 27
ISR_NoError 28
ISR_WithError 29
ISR_WithError 30
ISR_NoError 31
ISR_NoError 32
ISR_NoError 33
ISR_NoError 34
ISR_NoError 35
ISR_NoError 36
ISR_NoError 37
ISR_NoError 38
ISR_NoError 39
ISR_NoError 40
ISR_NoError 41
ISR_NoError 42
ISR_NoError 43
ISR_NoError 44
ISR_NoError 45
ISR_NoError 46
ISR_NoError 47
ISR_NoError 128

section .rodata

align 16
IDTR:
  dw 1023
  dd 0

