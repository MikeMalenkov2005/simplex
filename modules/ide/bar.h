#ifndef _BAR_H
#define _BAR_H

typedef unsigned long HBAR;

HBAR BAR_Init(int device, int index);
void BAR_Free(HBAR base);

int BAR_GetByte(HBAR base, unsigned offset);
int BAR_SetByte(HBAR base, unsigned offset, int value);

#endif

