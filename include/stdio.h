#ifndef _STDIO_H
#define _STDIO_H

#include <stddef.h>

typedef struct FILE FILE;

struct fpos_t
{
  unsigned int low;
  unsigned int high;
};

typedef struct fpos_t fpos_t;

#define BUFSIZ  512

#define EOF (-1)

#define FOPEN_MAX 16

#define FILENAME_MAX  260
#define L_tmpnam  FOPEN_MAX

#define SEEK_CUR  1
#define SEEK_END  2
#define SEEK_SET  0

#define TMP_MAX 0x7FFFFFFF

FILE *__crt_get_file(int id);

#define stderr  __crt_get_file(2)
#define stdin   __crt_get_file(0)
#define stdout  __crt_get_file(1)

int remove(const char *filename);
int rename(const char *old, const char *new);

FILE *tmpfile(void);
char *tmpnam(char *s);

int fclose(FILE *stream);
int fflush(FILE *stream);

FILE *fopen(const char *filename, const char *mode);
FILE *freopen(const char *filename, const char *mode, FILE *stream);

void setbuf(FILE *stream, char *buf);
void setvbuf(FILE *stream, char *buf, int mode, size_t size);

int fprintf(FILE *stream, const char *format, ...);
int fscanf(FILE *stream, const char *format, ...);

int printf(FILE *stream, const char *format, ...);
int scanf(FILE *stream, const char *format, ...);

int snprintf(char *s, size_t n, const char *format, ...);
int sprintf(char *s, const char *format, ...);
int sscanf(char *s, const char *format, ...);

/* vfprintf */

#endif

