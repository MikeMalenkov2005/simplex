#ifndef _PS2_CMD_H
#define _PS2_CMD_H

#include <sys/types.h>

#define BUFFER_SIZE 512
#define BUFFER_MASK (BUFFER_SIZE - 1)

typedef struct PS2_Buffer
{
  int head, tail, changed;
  K_U8 data[BUFFER_SIZE];
} PS2_Buffer;

#define COMMAND_SIZE  10

typedef struct PS2_Command
{
  int sender;
  K_U8 length;
  K_U8 returns;
  K_U8 data[COMMAND_SIZE];
} PS2_Command;

#define QUEUE_SIZE  16
#define QUEUE_MASK  (QUEUE_SIZE - 1)

typedef struct PS2_Queue
{
  int head, tail;
  PS2_Command commands[QUEUE_SIZE];
} PS2_Queue;

#endif

