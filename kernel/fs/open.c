#include <kernel/fs.h>
#include <kernel/types.h>
#include <kernel/uaccess.h>

#define ENOENT           2      /* No such file or directory */
#define ENFILE          23      /* File table overflow */

long sys_open(const char *user_pathname, int flags, mode_t mode) {
    char kernel_pathname[MAX_PATH_LEN];
    
    /* 
     * Step 1: Safely copy the file path from user memory space 
     * to a protected kernel buffer.
     */
    int err = copy_str_from_user(kernel_pathname, user_pathname, MAX_PATH_LEN);
    if (err < 0) {
        return (long)err; /* Returns negative error (e.g. -EFAULT, -ENAMETOOLONG) */
    }
    
    /* 
     * Step 2: Request the VFS layer to open/create the file,
     * which returns an open file description object pointer.
     */
    struct file *file_obj = vfs_open(kernel_pathname, flags, mode);
    if (file_obj == NULL) {
        /* Simplification: Assume ENOENT if open fails */
        return -ENOENT;
    }
    
    /* 
     * Step 3: Find a free descriptor slot in the process's 
     * file descriptor table and bind it to the open file object.
     */
    int fd = fd_alloc(file_obj);
    if (fd < 0) {
        /* Release the file object reference if fd table was full */
        file_obj->ref_count = 0;
        return (long)fd; /* Returns -EMFILE */
    }
    
    /* 
     * Step 4: Return the index of the allocated file descriptor.
     */
    return (long)fd;
}
