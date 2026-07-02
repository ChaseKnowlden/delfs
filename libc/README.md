# Delfs Custom glibc Clone

This is a minimal glibc-compatible C library (`libc`) clone for the **Delfs** operating system on the **x86_64** architecture. It implements the directory structure pattern used by GNU libc.

## Directory Structure

The structure mimics the standard GNU glibc layout by splitting files between public headers (`include`), general/generic OS implementations (`sysdeps/generic`), and OS/architecture-specific overrides (`sysdeps/delfs/x86_64`).

```
libc/
├── Makefile                 # Builds libc.a and crt0.o
├── README.md                # This documentation
├── include/                 # Public library headers
│   ├── bits/
│   │   └── fcntl.h          # Architecture/OS-specific file flags (e.g. O_RDONLY, O_CREAT)
│   ├── sys/
│   │   ├── syscall.h        # System call number mappings (Linux/Delfs-compatible)
│   │   └── types.h          # POSIX system types (mode_t, size_t, ssize_t)
│   ├── errno.h              # Error number definitions and errno declaration
│   ├── fcntl.h              # File control declarations (open)
│   ├── stdio.h              # Standard IO declarations (fprintf, fflush, vfprintf)
│   ├── stdlib.h             # Standard library declarations (abort)
│   └── unistd.h             # POSIX API placeholders (read, write, close)
├── src/
│   └── errno.c              # Core global errno variable definition
├── stdio/
│   ├── fflush.c             # Stream flushing implementation
│   └── fprintf.c            # Formatted printing logic
├── stdlib/
│   └── abort.c              # Implementation of the abort function
└── sysdeps/                 # System-dependent overrides
    └── delfs/               # Delfs OS-specific overrides
        ├── x86_64/
        │   ├── start.S      # Assembly C runtime startup code (_start)
        │   └── syscall.h    # Inline assembly macros for x86_64 system calls
        └── write.c          # Syscall wrapper for write()
```

---

## How It Works

### 1. The `sysdeps` Search Order
Glibc uses a prioritized search order to locate code files during compilation. This clone replicates this behavior via compiler include flags in the `Makefile`:
```makefile
INCLUDES = -Isysdeps/delfs/x86_64 -Isysdeps/delfs -Isysdeps/generic -Iinclude
```

### 2. Startup and Entry Point (`crt0.o`)
Every executable needs an entry point. By default, the compiler looks for `_start` inside `crt0.o` (C Runtime 0).
- `sysdeps/delfs/x86_64/start.S` implements `_start`.
- It aligns the stack, sets up `argc` (from `(%rsp)`) and `argv` (from `8(%rsp)`), calls C `main()`, and then invokes the `exit` system call (`60`) with the exit code returned by `main`.

---

## Building

To compile the library, run:
```bash
make
```
This produces:
- `libc.a`: The static C library containing `errno`, `abort`, `fprintf`, `fflush`, and `write`.
- `crt0.o`: The C runtime startup object file.
