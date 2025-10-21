#ifndef _STDARG_H
#define _STDARG_H

/* I GAVE UP :(

typedef char *va_list;

#define __va_align(v) (((sizeof(v) + 3) & ~(unsigned long)3))

#define va_start(ap, arg) \
  ((ap) = (va_list)(&(arg)) + __va_align(arg))
#define va_arg(ap, type)  \
  (*(type*)((ap) += __va_align(type), (ap) - __va_align(type)))
#define va_end(ap, type)  \
  ((ap) = (va_list)0)
*/

typedef __builtin_va_list va_list;

#define va_start(ap, arg) __builtin_va_start(ap, arg)
#define va_arg(ap, type)  __builtin_va_arg(ap, type)
#define va_end(ap)        __builtin_va_end(ap)

#endif

