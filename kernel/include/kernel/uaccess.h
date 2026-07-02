#ifndef _KERNEL_UACCESS_H
#define _KERNEL_UACCESS_H

#include <kernel/types.h>

/*
 * Copy a block of data from user-space to kernel-space.
 * Returns 0 on success, or the number of bytes that could not be copied on failure.
 */
int copy_from_user(void *to, const void *from, size_t n);

/*
 * Safely copy a null-terminated string from user-space to kernel-space.
 * Returns 0 on success, -EFAULT if memory error, or -ENAMETOOLONG if limit exceeded.
 */
int copy_str_from_user(char *dst, const char *src, size_t max_len);

#endif /* _KERNEL_UACCESS_H */
