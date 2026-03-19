SIMPLEX
=======

SIMPLEX is a simplistic OS based on microkernel achitecture.

System Modules
--------------

General:
- `dm` is a driver manager for registring and finding drivers.
- `uart` is a UART driver for external terminal I/O.
- `pci` is a PCI driver that detects and manages PCI devices.

Only for x86:
- `x86vga` is a simple VGA text mode driver.
- `x86ps2` is a PS/2 keyboard driver.

Kernel Features
---------------

- System Module Loading
- Virtual Memory (without swap file)
- Shared Memory
- Message Passing
- Round Robin task scheduling
- Task Local Storage (only with syscalls)
- Multitasking and multithreading
- Task Signal Handling
- Modular IRQ Handling

Supported Platformrs
--------------------

- `x86` - IBM PC compatible 32-bit machine (at least i486 DX).

C Standard Library Implementation
---------------------------------

Current C standard version: ANSI

Fully implemented headers:
- `ctype.h` - `is*` functions
- `iso646.h` - operator macros
- `stddef.h` - standard typedefs and constants
- `setjmp.h` - setjmp and longjmp functions
- `limits.h` - standard limit constants
- `signal.h` - standard signalling functions
- `errno.h` - threadsafe errno variable
- `assert.h` - standard assertions
- `stdarg.h` - macros for builtins
- `float.h` - float constants
- `string.h` - `str*` and `mem*` functions
- `time.h` - timing functions and structs

Partially implemented headers:
- `stdlib.h` - most standard functions
- `stdio.h` - IO functions, types and macros

Not implemented headers:
- `locale.h` - locale functions and constants
- `math.h` - math functions and constants

Non Standard Library
--------------------

General:
- `sys/types.h` - kernel typedefs
- `sys/limits.h` - kernel limit constants
- `sys/syscall.h` - syscall function and constants
- `sys/memory.h` - memory syscall flags
- `sys/task.h` - task flags (returned by `sys_check`)
- `sys/elf.h` - ELF structs and constants
- `sys/pio.h` - port I/O operations (for drivers only)
- `drv/dm.h` - DM (driver manager) API functions
- `drv/pci.h` - PCI driver API structs and macros
- `drv/uart.h` - UART driver API structs and macros
- `simplex.h` - syscall macros (`sys_*`)

Only for x86:
- `drv/x86ps2.h` - x86 PS/2 driver structs and macros
- `drv/x86vga.h` - x86 VGA driver structs and macros

POSIX Standard implementation
-----------------------------

Currently POSIX is not supported.

