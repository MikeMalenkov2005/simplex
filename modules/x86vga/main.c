#include <drv/x86vga.h>
#include <sys/memory.h>
#include <sys/limits.h>
#include <sys/types.h>
#include <x86/pio.h>
#include <simplex.h>
#include <string.h>
#include <time.h>

static K_U16 *text_screen;
static K_U16 text_cursor;
static K_U8 text_color = 7;

static void VGA_FillU16(K_U16 *buffer, K_U16 value, K_USIZE size)
{
  while (size--) *buffer++ = value;
}

static void VGA_UpdateTextCursor()
{
  if (!(text_cursor & 0x8000))
  {
    PIO_Write8(0x3D4, 0x0F);
    PIO_Write8(0x3D5, text_cursor);
    PIO_Write8(0x3D4, 0x0E);
    PIO_Write8(0x3D5, text_cursor >> 8);
  }
}

static void VGA_ShowTextCursor(K_U8 Start, K_U8 End)
{
  PIO_Write8(0x3D4, 0x0A);
  PIO_Write8(0x3D5, Start & 0x1F);
  PIO_Write8(0x3D4, 0x0B);
  PIO_Write8(0x3D5, End & 0x1F);
  text_cursor &= 0x7FFF;
}

static void VGA_HideTextCursor()
{
  PIO_Write8(0x3D4, 0x0A);
  PIO_Write8(0x3D5, 0x20);
  text_cursor |= 0x8000;
}

static void VGA_ClearScreen()
{
  text_cursor &= 0x8000;
  VGA_FillU16(text_screen, text_color << 8, 2000);
  VGA_UpdateTextCursor();
}

static void VGA_Scroll(K_U8 lines)
{
  const size_t chars = lines * 80;
  if (lines < 25)
  {
    memcpy(text_screen, text_screen + (chars << 1), 4000 - (chars << 1));
    VGA_FillU16(text_screen + 2000 - chars, text_color << 8, chars);
    if ((text_cursor & 0x7FFF) > chars) text_cursor -= chars;
    else text_cursor = (text_cursor & 0x8000) | ((text_cursor & 0x7FFF) % 80);
    VGA_UpdateTextCursor();
  }
  else VGA_ClearScreen();
}

int main()
{
  int tid = -1, i;
  K_U8 message[K_MESSAGE_SIZE];

  text_screen = sys_map(0xB8000, 4000, MAP_RD | MAP_WR);
  VGA_ShowTextCursor(14, 15);
  VGA_ClearScreen();

  time_t t = time(NULL);
  char *s = ctime(&t);
  while (*s)
  {
    if (*s == '\n')
    {
      text_cursor += 80 - (text_cursor & 0x7FFF) % 80;
      ++s;
    }
    else text_screen[text_cursor++ & 0x7FFF] = *s++ | (text_color << 8);
  }
  VGA_UpdateTextCursor();
  
  while ((tid = sys_wait(&message, -1))) if (tid != -1)
  {
    for (i = 0; i < K_MESSAGE_SIZE && message[i]; ++i) switch (message[i++])
    {
    case VGA_PUT_STRING:
      while (i < K_MESSAGE_SIZE && message[i])
      {
        text_screen[text_cursor & 0x7FFF] = message[i++] | (text_color << 8);
        if ((++text_cursor & 0x7FFF) >= 2000) VGA_Scroll(1);
      }
      VGA_UpdateTextCursor();
      break;
    case VGA_PUT_SYMBOL:
      if (i < K_MESSAGE_SIZE)
      {
        text_screen[text_cursor & 0x7FFF] = message[i] | (text_color << 8);
        if ((++text_cursor & 0x7FFF) >= 2000) VGA_Scroll(1);
        VGA_UpdateTextCursor();
      }
      break;
    case VGA_SET_SYMBOL:
      if (i < K_MESSAGE_SIZE)
      {
        text_screen[text_cursor & 0x7FFF] = message[i] | (text_color << 8);
      }
      break;
    case VGA_SET_CURSOR:
      if (++i < K_MESSAGE_SIZE)
      {
        message[i] %= 25;
        message[i - 1] %= 80;
        text_cursor = (text_cursor & 0x8000)
          | (message[i] * 80 + message[i - 1]);
        VGA_UpdateTextCursor();
      }
      break;
    case VGA_SET_COLOR:
      if (i < K_MESSAGE_SIZE) text_color = message[i];
      break;
    case VGA_SHOW_CURSOR:
      if (++i < K_MESSAGE_SIZE) VGA_ShowTextCursor(message[i - 1], message[i]);
      break;
    case VGA_HIDE_CURSOR:
      VGA_HideTextCursor();
      --i;
      break;
    case VGA_CLEAR_SCREEN:
      VGA_ClearScreen();
      --i;
      break;
    case VGA_SCROLL:
      if (i < K_MESSAGE_SIZE) VGA_Scroll(message[i]);
      break;
    }
  }
  return 0;
}

