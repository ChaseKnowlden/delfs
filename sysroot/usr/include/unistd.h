#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Standard POSIX system call wrapper declarations */
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
int close(int fd);

#ifdef __cplusplus
}
#endif

#endif /* _UNISTD_H */
