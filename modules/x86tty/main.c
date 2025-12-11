#include <sys/limits.h>
#include <drv/x86vga.h>
#include <drv/x86ps2.h>
#include <simplex.h>

void putch(int ch)
{
  K_U8 output[K_MESSAGE_SIZE] = VGA_PUT_SYMBOL_S;
  output[1] = ch;
  sys_send(output, VGA_TASK_ID);
}

int getch(void)
{
  static K_U8 input[K_MESSAGE_SIZE];
  static K_U8 index;
  while (index >= input[0])
  {
    index = 0;
    input[0] = PS2_RECEIVE;
    input[1] = 0;
    if (sys_send(input, PS2_TASK_ID) == -1)
    {
      input[0] = 0;
      return -1;
    }
  }
  return input[++index];
}

int main(void)
{
  int ch;
  putch('>');
  putch(' ');
  while ((ch = getch()) != -1) putch(ch);
  putch('E');
  putch('X');
  putch('I');
  putch('T');
  return 0;
}

