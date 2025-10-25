#ifndef _SHELL_H
#define _SHELL_H

#include <stddef.h>

int SH_Flush();

int SH_GetChar();

int SH_EchoChar();

int SH_PutChar(int ch);

size_t SH_GetString(char *buf, size_t size);

size_t SH_EchoString(char *buf, size_t size);

size_t SH_PutString(const char *str, size_t size);

#endif

