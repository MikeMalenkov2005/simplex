#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <types.h>

void K_SystemCallDispatch(K_USIZE index, K_USIZE arg1, K_USIZE arg2, K_USIZE arg3);

#endif

