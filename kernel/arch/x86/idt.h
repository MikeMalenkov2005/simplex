#ifndef _X86_IDT_H
#define _X86_IDT_H

#include <types.h>

void IDT_Load();

void IDT_SetEntry(K_U8 index, K_USIZE address, K_U16 selector, K_U8 access);

void IDT_Init();

#endif

