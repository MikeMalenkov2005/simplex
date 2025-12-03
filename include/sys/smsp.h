#ifndef _SYS_SMSP_H
#define _SYS_SMSP_H

#include <sys/types.h>
#include <sys/limits.h>

#define SMSP_FAILURE  (~(K_U32)0)
#define SMSP_SUCCESS  0

#define SMSP_OPEN   1
#define SMSP_CLOSE  2
#define SMSP_READ   3
#define SMSP_WRITE  4
#define SMSP_SEEK   5
#define SMSP_IOCTL  6

#define SMSP_CREATE_BIT   1
#define SMSP_NOREPLY_BIT  2
#define SMSP_APPEND_BIT   4
#define SMSP_RESTRICT_BIT 8
#define SMSP_TRUNCATE_BIT 16

typedef union SMSP_Payload SMSP_Payload;

struct SMSP
{
  K_HANDLE Buffer;
  K_USIZE Size;
  K_U32 StreamID;
  K_U16 Command;
  K_U16 Flags;
  K_U8 Reserved[K_MESSAGE_SIZE - sizeof(K_HANDLE) - sizeof(K_USIZE) - 8];
};

typedef struct SMSP SMSP;

#endif

