#ifndef _STDIO_H
#define _STDIO_H

#include <sys/types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef EOF
#define EOF (-1)
#endif

/* Standard IO File structure representation */
struct _IO_FILE {
    int fd;
};

typedef struct _IO_FILE FILE;

/* Standard stream pointers */
extern FILE *stdout;
extern FILE *stderr;

/* Formatted output functions */
int fprintf(FILE *stream, const char *format, ...);
int vfprintf(FILE *stream, const char *format, va_list ap);
int fflush(FILE *stream);


#ifdef __cplusplus
}
#endif

#endif /* _STDIO_H */
