#ifndef _K_SYSCALL_H
#define _K_SYSCALL_H

#include <sys/types.h>

void K_SystemCallDispatch(K_USIZE index, K_USIZE arg1, K_USIZE arg2, K_USIZE arg3);

#endif

