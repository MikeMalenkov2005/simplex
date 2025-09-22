#ifndef _ERRNO_H
#define _ERRNO_H

/* TODO: Make POSIX compatible! */

#define EDOM    1 /* Math argument out of the function's domaim */
#define EILSEQ  2 /* Illegal byte sequence */
#define ERANGE  3 /* Math result is not representable */

int *_errno(void);

#define errno (*_errno())

#endif

