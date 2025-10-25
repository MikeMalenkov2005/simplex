#include "shell.h"

#include <signal.h>
#include <string.h>

#include <simplex.h>
#include <drv/uart.h>

static char buffer[] = "\r\nHello, World!\r\n";

void sigint_handler(int sig)
{
  (void)sig;
  SH_PutString(buffer, strlen(buffer));
}

int main()
{
  signal(SIGINT, sigint_handler);
  raise(SIGINT);
  for (;;)
  {
    int ch = SH_GetChar();
    if (ch != -1)
    {
      SH_PutChar((ch >> 4) < 10 ? (ch >> 4) | '0' : (ch >> 4) + 'A' - 10);
      SH_PutChar((ch & 15) < 10 ? (ch & 15) | '0' : (ch & 15) + 'A' - 10);
      SH_PutChar(' ');
    }
    else SH_PutString("-1 ", 3);
    SH_Flush();
  }
  return 0;
}

