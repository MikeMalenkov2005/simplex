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

void *memchr(const void *s, int c, size_t n);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);

size_t strlen(const char *s);

int strcoll(const char *s1, const char *s2);
size_t strxfrm(char *s1, const char *s2, size_t n);

size_t strcspn(const char *s1, const char *s2);
size_t strspn(const char *s1, const char *s2);
char *strpbrk(const char *s1, const char *s2);

char *strstr(const char *s1, const char *s2);
char *strtok(char *s1, const char *s2);

char *strerror(int errnum);

#endif

