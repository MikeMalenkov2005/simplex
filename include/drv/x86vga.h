#ifndef _DRV_X86VGA_H
#define _DRV_X86VGA_H

#include <sys/limits.h>
#include <sys/types.h>

#define VGA_PUT_STRING    1   /* (symbol..., 0) */
#define VGA_PUT_SYMBOL    2   /* (symbol)       */
#define VGA_SET_SYMBOL    3   /* (symbol)       */
#define VGA_SET_CURSOR    4   /* (x, y)         */
#define VGA_SET_COLOR     5   /* (color)        */
#define VGA_SHOW_CURSOR   6   /* (start, end)   */
#define VGA_HIDE_CURSOR   7   /* ()             */
#define VGA_CLEAR_SCREEN  8   /* ()             */
#define VGA_SCROLL        9   /* (lines)        */
#define VGA_GET_INFO      10  /* ()             */

#define VGA_PUT_STRING_S    "\001"
#define VGA_PUT_SYMBOL_S    "\002"
#define VGA_SET_SYMBOL_S    "\003"
#define VGA_SET_CURSOR_S    "\004"
#define VGA_SET_COLOR_S     "\005"
#define VGA_SHOW_CURSOR_S   "\006"
#define VGA_HIDE_CURSOR_S   "\007"
#define VGA_CLEAR_SCREEN_S  "\010"
#define VGA_SCROLL_S        "\011"
#define VGA_GET_INFO_S      "\012"

#define VGA_CURSOR_HIDDEN_BIT     0x8000
#define VGA_CURSOR_POSITION_MASK  0x7FFF

typedef struct VGA_Info
{
  K_U16 rows;
  K_U16 columns;
  K_U16 cursor;
  K_U8 color;
  K_U8 reserved;
  K_U8 padding[K_MESSAGE_SIZE - 8];
} VGA_Info;

#define VGA_TASK_ID 3

#endif

