#ifndef _DRV_DM_H
#define _DRV_DM_H

#include <sys/types.h>
#include <sys/limits.h>

#define DM_FIND     1
#define DM_REGISTER 2

#define DM_FIND_S     "\001"
#define DM_REGISTER_S "\002"

union DM_Packet
{
  int Result;
  K_U8 Command;
  K_U8 String[K_MESSAGE_SIZE];
};

typedef union DM_Packet DM_Packet;

#define DM_TASK_ID 1

/**
 * @param name The name of a registered driver.
 * @return -1 on failure and a task id on success.
 */
int DM_Find(const char *name);

/**
 * @param name The name of the driver to register.
 * @return 0 on failure and a non-zero value on success.
 */
int DM_Register(const char *name);

#endif

