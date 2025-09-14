#ifndef _X86_PIO_H
#define _X86_PIO_H

#include <types.h>

K_U8 PIO_Read(K_U16 port);
void PIO_Write(K_U16 port, K_U8 byte);

#endif

