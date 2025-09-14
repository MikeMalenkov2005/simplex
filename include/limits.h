#ifndef _LIMITS_H
#define _LIMITS_H

#if defined(__x86_64__)
#define _LONG_64BITS
#elif defined(__arm__)
#define _LONG_32BITS
#define _CHAR_UNSIGNED
#endif

#define CHAR_BIT  8

#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 0xFF

#ifndef _CHAR_UNSIGNED
#define CHAR_MIN  SCHAR_MIN
#define CHAR_MAX  SCHAR_MAX
#else
#define CHAR_MIN  0
#define CHAR_MAX  UCHAR_MAX
#endif

#define LB_LEN_MAX  5

#define SHRT_MIN  (-0x7FFF - 1)
#define SHRT_MAX    0x7FFF
#define USHRT_MAX   0xFFFF

#define INT_MIN   (-0x7FFFFFFF - 1)
#define INT_MAX     0x7FFFFFFF
#define UINT_MAX    0xFFFFFFFFU

#ifndef _LONG_64BITS
#define LONG_MIN  (-0x7FFFFFFFL - 1)
#define LONG_MAX    0x7FFFFFFFL
#define ULONG_MAX   0xFFFFFFFFUL
#else
#define LONG_MIN  (-0x7FFFFFFFFFFFFFFFL - 1)
#define LONG_MAX    0x7FFFFFFFFFFFFFFFL
#define ULONG_MAX   0xFFFFFFFFFFFFFFFFUL
#endif

#endif

