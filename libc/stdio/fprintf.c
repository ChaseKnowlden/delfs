#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

/* Instantiate stdout and stderr static stream configurations */
static FILE _stdout = { .fd = 1 };
static FILE _stderr = { .fd = 2 };

FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

/* Helper to convert unsigned long to string */
static void utoa(unsigned long val, char *buf, int base, int uppercase) {
    char *p = buf;
    if (val == 0) {
        *p++ = '0';
        *p = '\0';
        return;
    }
    while (val > 0) {
        int digit = val % base;
        if (digit < 10) {
            *p++ = '0' + digit;
        } else {
            *p++ = (uppercase ? 'A' : 'a') + (digit - 10);
        }
        val /= base;
    }
    *p = '\0';
    
    /* Reverse the buffer string */
    int len = p - buf;
    for (int i = 0; i < len / 2; i++) {
        char tmp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = tmp;
    }
}

/* Helper to convert signed long to string */
static void itoa(long val, char *buf) {
    if (val < 0) {
        *buf++ = '-';
        utoa((unsigned long)-val, buf, 10, 0);
    } else {
        utoa((unsigned long)val, buf, 10, 0);
    }
}

int vfprintf(FILE *stream, const char *format, va_list ap) {
    if (!stream || !format) return -1;

    int count = 0;
    const char *p = format;
    char num_buf[65];

    while (*p) {
        if (*p != '%') {
            char c = *p;
            if (write(stream->fd, &c, 1) < 0) return -1;
            count++;
            p++;
            continue;
        }

        p++; /* Skip '%' */
        if (!*p) break;

        switch (*p) {
            case 'c': {
                char c = (char)va_arg(ap, int);
                if (write(stream->fd, &c, 1) < 0) return -1;
                count++;
                break;
            }
            case 's': {
                const char *s = va_arg(ap, const char *);
                if (!s) s = "(null)";
                int len = 0;
                while (s[len]) len++;
                if (write(stream->fd, s, len) < 0) return -1;
                count += len;
                break;
            }
            case 'd':
            case 'i': {
                long val = va_arg(ap, int);
                itoa(val, num_buf);
                int len = 0;
                while (num_buf[len]) len++;
                if (write(stream->fd, num_buf, len) < 0) return -1;
                count += len;
                break;
            }
            case 'u': {
                unsigned int val = va_arg(ap, unsigned int);
                utoa(val, num_buf, 10, 0);
                int len = 0;
                while (num_buf[len]) len++;
                if (write(stream->fd, num_buf, len) < 0) return -1;
                count += len;
                break;
            }
            case 'x':
            case 'X': {
                unsigned int val = va_arg(ap, unsigned int);
                utoa(val, num_buf, 16, (*p == 'X'));
                int len = 0;
                while (num_buf[len]) len++;
                if (write(stream->fd, num_buf, len) < 0) return -1;
                count += len;
                break;
            }
            case 'p': {
                void *ptr = va_arg(ap, void *);
                if (write(stream->fd, "0x", 2) < 0) return -1;
                count += 2;
                utoa((unsigned long)ptr, num_buf, 16, 0);
                int len = 0;
                while (num_buf[len]) len++;
                if (write(stream->fd, num_buf, len) < 0) return -1;
                count += len;
                break;
            }
            case '%': {
                char c = '%';
                if (write(stream->fd, &c, 1) < 0) return -1;
                count++;
                break;
            }
            default: {
                char c = '%';
                if (write(stream->fd, &c, 1) < 0) return -1;
                count++;
                c = *p;
                if (write(stream->fd, &c, 1) < 0) return -1;
                count++;
                break;
            }
        }
        p++;
    }
    return count;
}

int fprintf(FILE *stream, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int ret = vfprintf(stream, format, ap);
    va_end(ap);
    return ret;
}
