#ifndef _SYS_MEMORY_H
#define _SYS_MEMORY_H

#define MAP_NO  0
#define MAP_RD  1
#define MAP_WR  2
#define MAP_EX  4

#ifdef __CRT_MAP_FLAGS
#define MAP_UP  8 /* MAY CONFLICT WITH THE C RUNTIME */
#endif

#endif

