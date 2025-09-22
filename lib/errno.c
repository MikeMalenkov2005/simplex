#include <errno.h>

#include "crt.h"

int *_errno(void)
{
  return &__crt_get_info()->__crt_errno;
}

