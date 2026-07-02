#include <stdlib.h>

void abort(void) {
    /* 
     * In standard UNIX/Linux-like environments, abort() raises the SIGABRT signal.
     * Since this is a freestanding environment for a custom OS, we first attempt
     * to invoke the exit system call (SYS_exit = 60 on x86_64) with exit status 134.
     * (134 represents 128 + SIGABRT, standard for a process terminated by SIGABRT).
     */
    __asm__ __volatile__ (
        "movq $60, %%rax\n\t"   /* SYS_exit */
        "movq $134, %%rdi\n\t"  /* Exit status 134 */
        "syscall"
        :
        :
        : "rax", "rdi", "rcx", "r11", "memory"
    );

    /* 
     * If the exit system call somehow returns or fails (which should never happen),
     * we trigger a hardware exception/crash using a trap instruction.
     * __builtin_trap() compiles to the 'ud2' instruction on x86_64.
     */
    while (1) {
        __builtin_trap();
    }
}
