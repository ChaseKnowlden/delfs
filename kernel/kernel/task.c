#include <kernel/task.h>
#include <kernel/types.h>

#define EMFILE 24 /* Too many open files */

/* Statically allocate a dummy init process for the Delfs OS kernel simulation */
static struct task_struct current_task = {
    .pid = 1,
    .comm = "init",
    .files = { .fds = {NULL} } /* Descriptor table initialized to empty */
};

struct task_struct *get_current_task(void) {
    return &current_task;
}

int fd_alloc(struct file *file_obj) {
    struct task_struct *curr = get_current_task();
    
    /* 
     * Start allocating file descriptors from 3 onwards, 
     * preserving 0 (stdin), 1 (stdout), and 2 (stderr).
     */
    for (int i = 3; i < MAX_FD; i++) {
        if (curr->files.fds[i] == NULL) {
            curr->files.fds[i] = file_obj;
            return i;
        }
    }
    return -EMFILE;
}

struct file *fd_get(int fd) {
    if (fd < 0 || fd >= MAX_FD) {
        return NULL;
    }
    return get_current_task()->files.fds[fd];
}

void fd_free(int fd) {
    if (fd >= 0 && fd < MAX_FD) {
        get_current_task()->files.fds[fd] = NULL;
    }
}
