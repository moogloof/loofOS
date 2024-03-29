CC=gcc
AS=as
CFLAGS=-Wall -O0 -ffreestanding -nostdlib -nostartfiles
ASFLAGS=-Iinclude -O0
BUILD_DIR=../../build
OBJ_DIR=../../obj
ARCH_DIR=../../arch
LIBC_DIR=../../libc
CUR_ARCH=arm

# Standard library specific build info
LIBC_SRCS=$(shell find $(LIBC_DIR) -name *.c -or -name *.s)
LIBC_OBJS=$(LIBC_SRCS:%=$(OBJ_DIR)/%.o)

# Arch specific build info
ARM_SRCS=$(shell find $(ARCH_DIR)/arm -name *.c -or -name *.s)
ARM_OBJS=$(ARM_SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/arm/kernel7.img

# Build kernel image
$(BUILD_DIR)/arm/kernel7.img: $(ARM_OBJS) $(LIBC_OBJS)
	mkdir -p $(dir $@)
	ld -o $@ $^ -T $(ARCH_DIR)/arm/linker.ld

# Build C files
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $< -Iinclude -I$(ARCH_DIR)/$(CUR_ARCH)

# Build assembly files
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) -o $@ $< $(ASFLAGS)
