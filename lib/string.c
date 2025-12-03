#include <string.h>
#include <limits.h>
#include <errno.h>

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

void *memchr(const void *s, int c, size_t n)
{
  const unsigned char *p = s;
  while (n--)
  {
    if (*p == (unsigned char)c) return (void*)p;
    ++p;
  }
  return NULL;
}

char *strchr(const char *s, int c)
{
  while (*s)
  {
    if (*s == (char)c) return (void*)s;
    ++s;
  }
  return NULL;
}

char *strrchr(const char *s, int c)
{
  const char *result = NULL;
  while (*s)
  {
    if (*s == (char)c) result = s;
    ++s;
  }
  return (void*)result;
}

size_t strlen(const char *s)
{
  size_t l = 0;
  while (*s++) ++l;
  return l;
}

int strcoll(const char *s1, const char *s2)
{
  /* TODO: USE LC_COLLATE LOCALE
  char *locale = setlocale(LC_COLLATE, NULL); */
  return strcmp(s1, s2);
}

size_t strxfrm(char *s1, const char *s2, size_t n)
{
  size_t i;
  /* TODO: USE LC_COLLATE LOCALE
  char *locale = setlocale(LC_COLLATE, NULL); */
  if (!n) return strlen(s2);
  if (!s1) return 0;
  for (i = 1; i < n && *s2; ++i) *s1++ = *s2++;
  *s1 = 0;
  return i - 1;
}

size_t strcspn(const char *s1, const char *s2)
{
  const unsigned char *p = (const void*)s2;
  char lut[1 << CHAR_BIT] = { 0 };
  size_t i;
  for (i = 0; p[i]; ++i) lut[p[i]] = 1;
  p = (const void*)s1;
  for (i = 0; p[i] && !lut[p[i]]; ++i);
  return i;
}

size_t strspn(const char *s1, const char *s2)
{
  const unsigned char *p = (const void*)s2;
  char lut[1 << CHAR_BIT] = { 0 };
  size_t i;
  for (i = 0; p[i]; ++i) lut[p[i]] = 1;
  p = (const void*)s1;
  for (i = 0; p[i] && lut[p[i]]; ++i);
  return i;
}

char *strpbrk(const char *s1, const char *s2)
{
  size_t i = strcspn(s1, s2);
  return s1[i] ? (void*)(s1 + i) : NULL;
}

char *strstr(const char *s1, const char *s2)
{
  size_t i;
  if (!s1 || !s2) return NULL;
  if (!s2[0]) return (void*)s1;
  if (!s2[1]) return strchr(s1, *s2);
  for (;;)
  {
    for (i = 0; s2[i] && s1[i] == s2[i]; ++i);
    if (!s2[i]) return (void*)s1;
    if (!s1[i]) return NULL;
    ++s1;
  }
}

char *strtok(char *s1, const char *s2)
{
  static char *s = NULL;
  static char *e = NULL;
  if (s1)
  {
    s = s1 + strspn(s1, s2);
    if (!*s) s = e = NULL;
  }
  else s = e;
  if (s)
  {
    e = strpbrk(s, s2);
    if (e) *e = 0;
  }
  return s;
}

char *strerror(int errnum)
{
  static char buffer[128];
  /* TODO: USE LC_COLLATE LOCALE
  char *locale = setlocale(LC_ALL, NULL); */
  switch (errnum)
  {
  case 0:
    return strcpy(buffer, "No error");
  case EDOM:
    return strcpy(buffer, "Numerical argument out of domain");
  case EILSEQ:
    return strcpy(buffer, "Illegal byte sequence");
  case ERANGE:
    return strcpy(buffer, "Numerical result out of range");
  default:
    return strcpy(buffer, "Unknown error");
  }
}

