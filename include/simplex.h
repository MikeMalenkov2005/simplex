#ifndef _SIMPLEX_H
#define _SIMPLEX_H

#include <sys/syscall.h>

long __syscall(long number, long arg1, long arg2, long arg3);

#define __syscall0(number) __syscall(number, 0, 0, 0)
#define __syscall1(number, arg1) __syscall(number, arg1, 0, 0)
#define __syscall2(number, arg1, arg2) __syscall(number, arg1, arg2, 0)
#define __syscall3(number, arg1, arg2, arg3) __syscall(number, arg1, arg2, arg3)

#define __get_syscall(_1, _2, _3, _4, NAME, ...) NAME

#define syscall(...) __get_syscall(__VA_ARGS__, __syscall3, __syscall2, __syscall1, __syscall0)(__VA_ARGS__)

#define sys_exit(status) syscall(SYS_EXIT, status)

#define sys_send(msg, tid) ((int)syscall(SYS_SEND, (unsigned long)(msg), tid))
#define sys_poll(msg) ((int)syscall(SYS_POLL, (unsigned long)(msg)))
#define sys_wait(msg) ((int)syscall(SYS_WAIT, (unsigned long)(msg)))
#define sys_fire(msg, tid) ((int)syscall(SYS_FIRE, (unsigned long)(msg), tid))

#define sys_gettid() ((int)syscall(SYS_GET_TASK_ID))
#define sys_getgid() ((int)syscall(SYS_GET_GROUP_ID))
#define sys_getptid() ((int)syscall(SYS_GET_PARENT_ID))
#define sys_getltid() ((int)syscall(SYS_GET_LEADER_ID))

#define sys_thread(entry, stack) ((int)syscall(SYS_CREATE_THREAD, (unsigned long)(entry), stack))
#define sys_switch() syscall(SYS_SWITCH_TASK)

#define sys_signal(tid, sig) syscall(SYS_SIGNAL_TASK, tid, sig)
#define sys_handler(handler) ((void*)(unsigned long)syscall(SYS_SET_HANDLER, (unsigned long)(handler)))

#define sys_get_time() syscall(SYS_GET_TIME)
#define sys_set_time(value) syscall(SYS_SET_TIME, value)

#define sys_tls_new() syscall(SYS_TLS_NEW)
#define sys_tls_get(slot) ((void*)(unsigned long)syscall(SYS_TLS_GET, slot))
#define sys_tls_set(slot, value) syscall(SYS_TLS_GET, slot, (unsigned long)(value))

#define sys_group_exit(status) syscall(SYS_GROUP_EXIT, status)

#define sys_get_ticks() ((unsigned long)syscall(SYS_GET_TICKS))

#define sys_map(address, size, flags) ((void*)(unsigned long)syscall(SYS_MAP, (unsigned long)(address), size, flags))
#define sys_free(address, size) syscall(SYS_FREE, (unsigned long)(address), size)
#define sys_remap(address, size, flags) syscall(SYS_REMAP, (unsigned long)(address), size, flags)
#define sys_share(address, size, tid) ((void*)(unsigned long)syscall(SYS_SHARE, (unsigned long)(address), size, tid))
#define sys_unshare(address, tid) (!!sys_share(address, 0, tid))

#define sys_get_zone() syscall(SYS_GET_TIME_ZONE)
#define sys_set_zone(seconds) syscall(SYS_SET_TIME_ZONE, seconds)

#define sys_irq_wait(irq) syscall(SYS_IRQ_WAIT, irq)
#define sys_irq_exit() syscall(SYS_IRQ_EXIT)

#endif

