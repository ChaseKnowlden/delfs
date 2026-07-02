#
# Makefile for Delfs OS Kernel Subsystem
#

CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -O2 -ffreestanding -fno-stack-protector -Iinclude

SRCS = fs/open.c fs/vfs.c kernel/task.c kernel/uaccess.c sys/syscall.c
OBJS = $(SRCS:.c=.o)
LIBKERNEL_A = libkernel.a

.PHONY: all clean

all: $(LIBKERNEL_A)

# Build the kernel archive
$(LIBKERNEL_A): $(OBJS)
	$(AR) rcs $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIBKERNEL_A)
