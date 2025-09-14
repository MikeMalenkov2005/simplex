#ifndef _X86_UTILS_H
#define _X86_UTILS_H

#include "../../utils.h"

void K_WritePort8(K_U16 port, K_U8 value);
void K_WritePort16(K_U16 port, K_U16 value);
void K_WritePort32(K_U16 port, K_U32 value);

void K_ReadPort8(K_U16 port);
void K_ReadPort16(K_U16 port);
void K_ReadPort32(K_U16 port);

K_U32 K_GetCPUFlags();

K_USIZE K_GetCR2();

#endif

