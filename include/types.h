#ifndef _TYPES_H
#define _TYPES_H

typedef unsigned char   K_U8;
typedef unsigned short  K_U16;
typedef unsigned int    K_U32;

typedef signed char   K_S8;
typedef signed short  K_S16;
typedef signed int    K_S32;

typedef unsigned long K_USIZE;
typedef signed long   K_SSIZE;

typedef void *K_HANDLE;
typedef K_S32 K_BOOL;

union K_U64S
{
  K_USIZE Value;
  struct { K_U32 Low, High; }; /* TODO: Change for BigEndian! */
};

typedef union K_U64S K_U64S;

union K_S64S
{
  K_SSIZE Value;
  struct { K_U32 Low, High; }; /* TODO: Change for BigEndian! */
};

typedef union K_S64S K_S64S;

#define TRUE  1
#define FALSE 0

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

#endif

