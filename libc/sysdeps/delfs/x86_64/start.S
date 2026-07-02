.global _start
.extern main

.section .text

_start:
    /* Clear frame pointer to mark the top of the call stack */
    xorq %rbp, %rbp
    
    /* On entry, the stack contains [argc, argv[0], argv[1], ..., NULL] */
    /* Pop/read argc into %rdi (1st argument to main) */
    movq (%rsp), %rdi
    
    /* Calculate argv address and put in %rsi (2nd argument to main) */
    leaq 8(%rsp), %rsi
    
    /* Align stack to 16 bytes for x86_64 ABI compliance before calling C functions */
    andq $-16, %rsp
    
    /* Call the main function of the program */
    call main
    
    /* Call exit system call with main's return value */
    /* SYS_exit = 60, exit code is in %rdi (%rax holds main's return value) */
    movq %rax, %rdi
    movq $60, %rax       /* SYS_exit */
    syscall
