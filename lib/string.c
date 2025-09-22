#include <string.h>

char *strcpy(char *s1, const char *s2)
{
  char *result = s1;
  while ((*s1++ = *s2++));
  return result;
}

char *strncpy(char *s1, const char *s2, size_t n)
{
  char *result = s1;
  while (n-- && (*s1++ = *s2++));
  if (~n) while (n--) *s1++ = 0;
  return result;
}

char *strcat(char *s1, const char *s2)
{
  char *result = s1;
  while (*s1) s1++;
  while ((*s1++ = *s2++));
  return result;
}

char *strncat(char *s1, const char *s2, size_t n)
{
  char *result = s1;
  while (*s1) s1++;
  while (n-- && *s2) *s1++ = *s2++;
  *s1 = 0;
  return result;
}

int strcmp(const char *s1, const char *s2)
{
  int result;
  while (!(result = (int)*s1++ - (int)*s2++));
  return result;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  int result = 0;
  while (n-- && !(result = (int)*s1++ - (int)*s2++));
  return result;
}

void *memchr(void *s, int c, size_t n)
{
  unsigned char *p = s;
  while (n--)
  {
    if (*p == (unsigned char)c) return p;
    ++p;
  }
  return NULL;
}

char *strchr(char *s, int c)
{
  while (*s)
  {
    if (*s == (char)c) return s;
    ++s;
  }
  return NULL;
}

