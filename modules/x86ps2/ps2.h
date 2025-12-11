#ifndef _PS2_CMD_H
#define _PS2_CMD_H

#include <sys/types.h>

#define BUFFER_SIZE 512
#define BUFFER_MASK (BUFFER_SIZE - 1)

typedef struct PS2_Buffer
{
  volatile int info;
  K_U8 data[BUFFER_SIZE];
} PS2_Buffer;

#define COMMAND_SIZE 8

#endif

