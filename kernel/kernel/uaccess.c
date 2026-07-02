#include <kernel/uaccess.h>
#include <kernel/types.h>

#define EFAULT          14      /* Bad address */
#define ENAMETOOLONG    36      /* File name too long */

int copy_from_user(void *to, const void *from, size_t n) {
    if (from == NULL || to == NULL) {
        return n; /* Fails to copy anything */
    }
    
    char *d = (char *)to;
    const char *s = (const char *)from;
    
    /* Mock copy: In a real kernel, this accesses user memory through page tables
     * and relies on the MMU/page fault exception handler to recover from bad addresses.
     */
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return 0;
}

int copy_str_from_user(char *dst, const char *src, size_t max_len) {
    if (src == NULL || dst == NULL) {
        return -EFAULT;
    }
    
    size_t i = 0;
    while (i < max_len) {
        dst[i] = src[i];
        if (dst[i] == '\0') {
            return 0; /* String copied successfully */
        }
        i++;
    }
    
    /* If the string was too long, ensure it is null-terminated and return error */
    if (max_len > 0) {
        dst[max_len - 1] = '\0';
    }
    return -ENAMETOOLONG;
}
