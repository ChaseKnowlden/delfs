#include <kernel/fs.h>
#include <kernel/types.h>

#define MAX_MOCK_INODES 16
#define MAX_OPEN_FILES 32

/* Kernel-space inode table containing simulated files on disk */
static struct inode mock_inodes[MAX_MOCK_INODES];
static int inode_count = 0;

/* Global open file description table in the kernel */
static struct file open_files_pool[MAX_OPEN_FILES];

/* Helper to compare strings in freestanding kernel */
static int kstrcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

/* Helper to copy strings in freestanding kernel */
static void kstrcpy(char *dest, const char *src) {
    while ((*dest++ = *src++));
}

void vfs_init(void) {
    /* Populating virtual mock files */
    mock_inodes[0].inode_num = 1;
    kstrcpy(mock_inodes[0].name, "test_file.txt");
    mock_inodes[0].size = 128;
    mock_inodes[0].mode = 0644;

    mock_inodes[1].inode_num = 2;
    kstrcpy(mock_inodes[1].name, "system_log");
    mock_inodes[1].size = 1024;
    mock_inodes[1].mode = 0600;

    inode_count = 2;
}

struct file *vfs_open(const char *pathname, int flags, mode_t mode) {
    struct inode *found_inode = NULL;
    
    /* Search the virtual filesystem directory for a matching filename */
    for (int i = 0; i < inode_count; i++) {
        if (kstrcmp(mock_inodes[i].name, pathname) == 0) {
            found_inode = &mock_inodes[i];
            break;
        }
    }

    /* If the file doesn't exist, check if O_CREAT is requested */
    if (found_inode == NULL) {
        if (flags & O_CREAT) {
            if (inode_count >= MAX_MOCK_INODES) {
                return NULL; /* Out of inodes / disk space */
            }
            int idx = inode_count++;
            mock_inodes[idx].inode_num = idx + 1;
            kstrcpy(mock_inodes[idx].name, pathname);
            mock_inodes[idx].size = 0;
            mock_inodes[idx].mode = mode;
            found_inode = &mock_inodes[idx];
        } else {
            return NULL; /* File not found (ENOENT) */
        }
    }

    /* Locate a free slot in the kernel-wide open file table */
    struct file *file_obj = NULL;
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_files_pool[i].ref_count == 0) {
            file_obj = &open_files_pool[i];
            break;
        }
    }

    if (file_obj == NULL) {
        return NULL; /* Kernel file description table overflow (ENFILE) */
    }

    /* Populate the open file description */
    file_obj->inode = found_inode;
    file_obj->offset = 0;
    file_obj->flags = flags;
    file_obj->ref_count = 1;

    return file_obj;
}
