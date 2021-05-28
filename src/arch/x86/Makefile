CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-Wall -O0 -ffreestanding -I../../../include -Ix86 -m32
ASFLAGS=-I../../../include -O0 -f bin
BUILD_DIR=../../../build
OBJ_DIR=../../../obj
SRC_DIR=../../../src
LIBC_DIR=$(SRC_DIR)/libc

# Standard library specific build info
LIBC_SRCS=$(shell find $(LIBC_DIR) -name *.c -or -name *.s)
LIBC_OBJS=$(LIBC_SRCS:%=$(OBJ_DIR)/%.o)

# x86_64 specific build info
x86_SRCS=$(shell find . -name *.c -or -name *.s ! -name *boot.s)
x86_OBJS=$(x86_SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/x86_64/loof.img

$(BUILD_DIR)/x86_64/loof.img: $(BUILD_DIR)/x86_64/boot $(BUILD_DIR)/x86_64/kernel
	dd seek=0 if=$(BUILD_DIR)/x86_64/boot of=$@
	dd seek=3 if=$(BUILD_DIR)/x86_64/kernel of=$@

$(BUILD_DIR)/x86_64/boot:
	mkdir -p $(dir $@)
	$(AS) -o $@ $(SRC_DIR)/arch/x86/boot/boot.s $(ASFLAGS)

$(BUILD_DIR)/x86_64/kernel: $(x86_OBJS)
	mkdir -p $(dir $@)
	x86_64-elf-ld -o $@ $^ -e _kernel_main -m elf_i386
	x86_64-elf-objcopy -O binary $@ $@

# Build C files
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Build assembly files
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) -o $@ $< $(ASFLAGS)

