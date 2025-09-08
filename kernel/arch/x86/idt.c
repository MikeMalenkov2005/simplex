#include "idt.h"

#include "isr.h"
#include "task.h"
#include "utils.h"

#include "../../timer.h"
#include "../../syscall.h"

#define X_FOR_ISR \
  X(0 , 8, 0x8E)  \
  X(1 , 8, 0x8E)  \
  X(2 , 8, 0x8E)  \
  X(3 , 8, 0x8E)  \
  X(4 , 8, 0x8E)  \
  X(5 , 8, 0x8E)  \
  X(6 , 8, 0x8E)  \
  X(7 , 8, 0x8E)  \
  X(8 , 8, 0x8E)  \
  X(9 , 8, 0x8E)  \
  X(10, 8, 0x8E)  \
  X(11, 8, 0x8E)  \
  X(12, 8, 0x8E)  \
  X(13, 8, 0x8E)  \
  X(14, 8, 0x8E)  \
  X(15, 8, 0x8E)  \
  X(16, 8, 0x8E)  \
  X(17, 8, 0x8E)  \
  X(18, 8, 0x8E)  \
  X(19, 8, 0x8E)  \
  X(20, 8, 0x8E)  \
  X(21, 8, 0x8E)  \
  X(22, 8, 0x8E)  \
  X(23, 8, 0x8E)  \
  X(24, 8, 0x8E)  \
  X(25, 8, 0x8E)  \
  X(26, 8, 0x8E)  \
  X(27, 8, 0x8E)  \
  X(28, 8, 0x8E)  \
  X(29, 8, 0x8E)  \
  X(30, 8, 0x8E)  \
  X(31, 8, 0x8E)  \
  X(32, 8, 0x8E)  \
  X(33, 8, 0x8E)  \
  X(34, 8, 0x8E)  \
  X(35, 8, 0x8E)  \
  X(36, 8, 0x8E)  \
  X(37, 8, 0x8E)  \
  X(38, 8, 0x8E)  \
  X(39, 8, 0x8E)  \
  X(40, 8, 0x8E)  \
  X(41, 8, 0x8E)  \
  X(42, 8, 0x8E)  \
  X(43, 8, 0x8E)  \
  X(44, 8, 0x8E)  \
  X(45, 8, 0x8E)  \
  X(46, 8, 0x8E)  \
  X(47, 8, 0x8E)  \
  X(128, 8, 0xEE)

#define ISR_Name(i) ISR_ ## i

#define X(i, s, a) extern void ISR_Name(i)();
X_FOR_ISR
#undef X

K_U16 (*IDT)[4];

void IDT_Dispatch(ISR_Frame *frame)
{
  K_U32 index = frame->Index;
  TASK_SetFrame(frame);
  if (index < 32)
  {
    /* TODO: Handle Exceptions */
  }
  else if (index == 32) K_TickCallback();
  else if (index < 48) (void)K_BeginTaskIRQ(index);
  else if (index == 128) K_SystemCallDispatch(frame->Eax, frame->Ebx, frame->Ecx, frame->Eax);
  if (index >= 32 && index < 48)
  {
    if (index >= 40) K_WritePort8(0xA0, 0x20);
    K_WritePort8(0x20, 0x20);
  }
}

void IDT_SetEntry(K_U8 index, K_USIZE address, K_U16 selector, K_U8 access)
{
  IDT[index][0] = (K_U16)address;
  IDT[index][1] = selector;
  IDT[index][2] = (K_U16)access << 8;
  IDT[index][3] = (K_U16)(address >> 16);
}

void IDT_Init()
{
  IDT = K_ZeroMemory(NULL, 1024);
  ISR_Init();
#define X(i, s, a) IDT_SetEntry(i, (K_USIZE)ISR_Name(i), s, a);
  X_FOR_ISR
#undef X
  IDT_Load();
}

