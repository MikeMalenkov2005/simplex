#include <sys/limits.h>
#include <drv/x86vga.h>
#include <drv/x86ps2.h>
#include <simplex.h>

void putch(int ch)
{
  K_U8 output[K_MESSAGE_SIZE];
  output[0] = VGA_PUT_SYMBOL;
  output[1] = ch;
  output[2] = 0;
  sys_send(output, VGA_TASK_ID);
}

int getch(void)
{
  static K_U8 input[K_MESSAGE_SIZE];
  static K_U8 index;
  while (index >= input[0])
  {
    input[0] = PS2_RECEIVE;
    input[1] = 0;
    input[2] = 0;
    sys_send(input, PS2_TASK_ID);
    input[0] = index = 0;
    sys_wait(input, PS2_TASK_ID);
  }
  return input[++index];
}

int main(void)
{
  int ch;
  putch('>');
  putch(' ');
  while ((ch = getch())) putch(ch);
  return 0;
}

