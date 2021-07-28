CC=x86_64-elf-gcc
AS=nasm
CFLAGS=-Wall -O0 -ffreestanding -I./ -m32 -nostdlib -mgeneral-regs-only
ASFLAGS=-O0 -f bin
BUILD_DIR=../../build
OBJ_DIR=../../obj
LIBC_DIR=libc

# Standard library specific build info
LIBC_SRCS=$(shell find $(LIBC_DIR) -name *.c -or -name *.s)
LIBC_OBJS=$(LIBC_SRCS:%=$(OBJ_DIR)/%.o)

# x86 specific build info
x86_SRCS=$(shell find . -name *.c -or -name *.s ! -name *boot.s)
x86_OBJS=$(x86_SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/x86/loof.img

$(BUILD_DIR)/x86/loof.img: $(BUILD_DIR)/x86/boot $(BUILD_DIR)/x86/kernel
	dd seek=0 if=$(BUILD_DIR)/x86/boot of=$@
	dd seek=3 if=$(BUILD_DIR)/x86/kernel of=$@
	dd if=$@ of=$(BUILD_DIR)/x86/loof_padded.img bs=100m conv=sync

$(BUILD_DIR)/x86/boot:
	mkdir -p $(dir $@)
	$(AS) -o $@ boot/boot.s $(ASFLAGS)

$(BUILD_DIR)/x86/kernel: $(x86_OBJS)
	mkdir -p $(dir $@)
	x86_64-elf-ld -o $@ $^ -m elf_i386 -Tkernel_linker.ld
#	x86_64-elf-objcopy -O binary --adjust-vma 0x100000 $@ $@

# Build C files
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Build assembly files
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) -o $@ $< -f elf32

