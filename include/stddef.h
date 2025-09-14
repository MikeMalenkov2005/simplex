#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

typedef unsigned long size_t;
typedef signed long ptrdiff_t;
typedef short wchar_t;

#define offsetof(type, member) ((size_t)&(*(type*)NULL).member)

#endif

