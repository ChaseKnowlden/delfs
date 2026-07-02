#
# Makefile for Delfs OS Custom glibc Clone
#

CC = gcc
AR = ar
AS = gcc
CFLAGS = -Wall -Wextra -O2 -ffreestanding -fno-stack-protector
LDFLAGS = -nostdlib -static

# Include paths. The search order mirrors glibc's sysdeps priority:
# 1. OS-specific architecture-specific overrides (sysdeps/delfs/x86_64)
# 2. OS-specific generic overrides (sysdeps/delfs)
# 3. Generic/architecture-agnostic fallbacks (sysdeps/generic)
# 4. Public API headers (include)
INCLUDES = -Isysdeps/delfs/x86_64 -Isysdeps/delfs -Isysdeps/generic -Iinclude

LIBC_A = libc.a
CRT0_O = crt0.o

SRCS = src/errno.c stdlib/abort.c stdio/fprintf.c stdio/fflush.c sysdeps/delfs/write.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(LIBC_A) $(CRT0_O)

# Build the static library libc.a
$(LIBC_A): $(OBJS)
	$(AR) rcs $@ $(OBJS)

# Build the C runtime startup object (entry point)
$(CRT0_O): sysdeps/delfs/x86_64/start.S
	$(AS) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Build generic C object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIBC_A) $(CRT0_O)

