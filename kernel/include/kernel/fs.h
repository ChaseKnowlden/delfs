#ifndef _KERNEL_FS_H
#define _KERNEL_FS_H

#include <kernel/types.h>

#define MAX_FD 32
#define MAX_PATH_LEN 256

/* Open flags (mirrors user-space) */
#define O_RDONLY    0x0000
#define O_WRONLY    0x0001
#define O_RDWR      0x0002
#define O_CREAT     0x0040

/* Kernel representation of an Inode (file on disk/VFS) */
struct inode {
    unsigned long inode_num;
    char name[64];
    size_t size;
    mode_t mode;
    /* In a real kernel, this would point to file data/blocks */
};

/* Kernel representation of an Open File Description */
struct file {
    struct inode *inode;
    off_t offset;
    int flags;
    int ref_count;
};

/* Process File Descriptor Table */
struct file_desc_table {
    struct file *fds[MAX_FD];
};

/* --- Function Prototypes --- */

/* VFS interfaces */
void vfs_init(void);
struct file *vfs_open(const char *pathname, int flags, mode_t mode);

/* File descriptor helpers */
int fd_alloc(struct file *file_obj);
struct file *fd_get(int fd);
void fd_free(int fd);

/* Syscall handler for open */
long sys_open(const char *user_pathname, int flags, mode_t mode);

#endif /* _KERNEL_FS_H */
