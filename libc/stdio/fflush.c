#include <stdio.h>

int fflush(FILE *stream) {
    /* 
     * In this custom minimal libc implementation, stdout, stderr, and all other
     * file streams are unbuffered (the write system call is invoked immediately).
     * Therefore, there are no internal buffers to flush, and we simply return 0
     * to indicate success.
     *
     * If stream is NULL, the standard requires flushing all open output streams,
     * which is also a successful no-op.
     */
    (void)stream;
    return 0;
}
