#ifndef _ASSERT_H
#define _ASSERT_H

#ifdef NDEBUG
void _assert(int value, const char *expression, int line, const char *file);
#define assert(expression) _assert(expression, #expression, __LINE__, __FILE__)
#else
#define assert(ignored) ((void)0)
#endif

#endif

