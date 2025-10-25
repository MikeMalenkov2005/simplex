#ifndef _X86_PIO_H
#define _X86_PIO_H

#include <sys/types.h>

void PIO_Write8(K_U16 port, K_U8 value);
void PIO_Write16(K_U16 port, K_U8 value);
void PIO_Write32(K_U16 port, K_U8 value);

K_U8  PIO_Read8(K_U16 port);
K_U16 PIO_Read16(K_U16 port);
K_U32 PIO_Read32(K_U16 port);

#endif

