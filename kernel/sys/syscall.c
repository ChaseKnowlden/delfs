#include <kernel/fs.h>
#include <kernel/types.h>

#define SYS_open    2
#define SYS_exit    60

#define ENOSYS      38      /* Function not implemented */

/*
 * Kernel system call dispatcher.
 *
 * In a real OS kernel, the low-level assembly interrupt/syscall handler
 * (e.g., registered via the LSTAR MSR on x86_64) saves the CPU state,
 * extracts registers, and passes them to this dispatch function.
 */
long dispatch_syscall(long num, long arg1, long arg2, long arg3) {
    switch (num) {
        case SYS_open:
            return sys_open((const char *)arg1, (int)arg2, (mode_t)arg3);
            
        case SYS_exit:
            /* In a real kernel, this would call exit_group() or terminate the process */
            return arg1;
            
        default:
            return -ENOSYS;
    }
}
