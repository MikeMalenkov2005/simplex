#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

int memcmp(const void *lhs, const void *rhs, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
void *memset(void *dest, int ch, size_t count);

char *strcpy(char *s1, const char *s2);
char *strncpy(char *s1, const char *s2, size_t n);

char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, size_t n);

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

void *memchr(void *s, int c, size_t n);
char *strchr(char *s, int c);

#endif

