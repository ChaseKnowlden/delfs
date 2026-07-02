#ifndef _KERNEL_TASK_H
#define _KERNEL_TASK_H

#include <kernel/types.h>
#include <kernel/fs.h>

/* Process/Task control block */
struct task_struct {
    pid_t pid;
    char comm[16];
    struct file_desc_table files;
};

/* Retrieve the currently executing task/process */
struct task_struct *get_current_task(void);

#endif /* _KERNEL_TASK_H */
