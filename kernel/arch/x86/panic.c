#include <types.h>

void K_Debug(K_U16 x, K_U16 y, K_U8 *string)
{
  K_U16 *screen = (K_HANDLE)0xB8000;
  K_U16 index = x + y * 80;
  while (*string) screen[index++] = 0x200 | *string++;
}

#define K_ToHex(c) (((c) & 15) < 10 ? ((c) & 15) + '0' : ((c) & 15) + 'A' - 10)

void K_DebugHex(K_U16 x, K_U16 y, K_U32 value)
{
  K_U8 string[9] = { 0 };
  K_U8 offset = sizeof string - 1;
  do { string[--offset] = K_ToHex(value); value >>= 4; } while (offset);
  K_Debug(x, y, string);
}

void K_Panic(const char *message)
{
  K_U16 *screen = (K_HANDLE)0xB8000;
  while (*message) *screen++ = 0x400 | (*message++ & 0xFF);
  for (;;);
}

