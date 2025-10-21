SIMPLEX
=======

SIMPLEX is a simplistic OS based on microkernel achitecture.

System Modules
--------------

- `uart` is a UART driver that uses the DSP for communication.
- `shell` is a simple command processor (shell).

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
- `sting.h` - `str*` and `mem*` functions
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

Partially implemented headers:
- `stdlib.h` - most standard functions

Not implemented headers:
- `locale.h` - locale functions and constants
- `stdio.h` - IO functions, types and macros
- `math.h` - math functions and constants
- `time.h` - timing functions and structs

Non Standard Library
--------------------

- `sys/types.h` - kernel typedefs
- `sys/limits.h` - kernel limit constants
- `sys/syscall.h` - syscall function and constants
- `sys/memory.h` - memory syscall flags
- `sys/dsp.h` - DSP structs and constants
- `sys/elf.h` - ELF structs and constants
- `drv/uart.h` - UART driver API structs and macros
- `simplex.h` - syscall macros (`sys_*`)

POSIX Standard implementation
-----------------------------

Currently POSIX is not supported.

Drived Streaming Protocol
-------------------------

The Drived Streaming Protocol (DSP) defines the general message structure
utilized for data streaming between through driver tasks.

The general packet structure:
1. 2-byte stream/device id
2. 1-byte action/packet id
3. 1-byte the payload size
4. the payload data

The stream id is used to distinguish multiple streams opened for one task.

The payload data depends on the action id and the payload size.

The payload size can be used to indicate if an error has occured.

If the action is any type of request except read or write,
the payload size should be set to zero by the client and ignored by the driver.

If the action is a reply then the payload size specifies,
how many bytes was successfuly read or written by the driver.

All action ids from 0 upto 127 are reserved by the protocol.
Action ids from 128 upto 255 can be utilized for driver-specific actions.

