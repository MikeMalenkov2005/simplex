#ifndef _CTYPE_H
#define _CTYPE_H

#define _C_DIGIT  0x01
#define _C_UPPER  0x02
#define _C_LOWER  0x04
#define _C_CNTRL  0x08
#define _C_XDIGIT 0x10
#define _C_PUNCT  0x20
#define _C_SPACE  0x40
#define _C_BLANK  0x80

extern const unsigned char _ctype[];

#define _ismask(c, m) ((_ctype[(c) & 0x7F] & (m)) != 0)

#define iscntrl(c) _ismask(c, _C_CNTRL)
#define isprint(c) (!iscntrl(c))

#define isspace(c) _ismask(c, _C_SPACE)
#define isblank(c) _ismask(c, _C_BLANK)

#define isgraph(c) (!_ismask(c, _C_CNTRL | _C_SPACE))
#define ispunct(c) _ismask(c, _C_PUNCT)

#define isalnum(c) _ismask(c, _C_DIGIT | _C_UPPER | _C_LOWER)
#define isalpha(c) _ismask(c, _C_UPPER | _C_LOWER)
#define isupper(c) _ismask(c, _C_UPPER)
#define islower(c) _ismask(c, _C_LOWER)

#define isdigit(c) ((((c) - '0') & 0x7F) < 10)
#define isodigit(c) ((((c) - '0') & 0x7F) < 8)
#define isxdigit(c) _ismask(c, _C_XDIGIT)

int tolower(int c);
int toupper(int c);

#endif

