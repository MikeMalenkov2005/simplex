#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef signed long ptrdiff_t;
typedef short wchar_t;

#define offsetof(type, member) ((size_t)&(*(type*)NULL).member)

#endif

