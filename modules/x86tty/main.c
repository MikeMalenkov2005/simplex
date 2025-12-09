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
  sys_send(output, 3);
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
    sys_send(input, 4);
    input[0] = index = 0;
    sys_wait(input, 4);
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

