#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

ssize_t write(int fd, const void *buf, size_t count) {
    unsigned long ret;
    
    /* 
     * Perform the write system call on x86_64.
     * Registers:
     * - rax: Syscall number (SYS_write = 1)
     * - rdi: fd (1st arg)
     * - rsi: buf (2nd arg)
     * - rdx: count (3rd arg)
     */
    __asm__ __volatile__ (
        "syscall"
        : "=a"(ret)
        : "a"(SYS_write), "D"(fd), "S"(buf), "d"(count)
        : "rcx", "r11", "memory"
    );
    
    /* Check for kernel error range [-4095, -1] */
    if (ret >= (unsigned long)-4095) {
        errno = (int)-(long)ret;
        return -1;
    }
    
    return (ssize_t)ret;
}
