#ifndef _SETJMP_H
#define _SETJMP_H

#if defined(__x86_64__)
#define _JMP_BUF_LEN  8 /* sysv preserved   */
#elif defined(i386) || defined(__i386__) || defined(__i386)
#define _JMP_BUF_LEN  6 /* cdecl preserved  */
#elif defined(__arm__)
#define _JMP_BUF_LEN  10
#else
#error setjmp: unknown architecture!
#endif

typedef long jmp_buf[_JMP_BUF_LEN];

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#endif

