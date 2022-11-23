LD=ld.lld
CC=clang
AS=nasm
LDFLAGS=--format=elf -O0 --nmagic -nostdlib --gc-sections --gdb-index
CFLAGS=-Wall -O0 -ffreestanding -I./ -m32 -nostdlib -mgeneral-regs-only -march=i386 --target=i386-unknown-linux-gnu -g
ASFLAGS=-O0 -f bin
BUILD_DIR=../../build
OBJ_DIR=../../obj
LIBC_DIR=libc
FSFLAGS=-input-charset iso8859-1 -no-emul-boot -boot-load-size 4 -boot-info-table

# Standard library specific build info
LIBC_SRCS=$(shell find $(LIBC_DIR) -name "*.c" -or -name "*.s")
LIBC_OBJS=$(LIBC_SRCS:%=$(OBJ_DIR)/%.o)

# x86 specific build info
x86_KERNEL_DIRS=./core ./drivers ./mm ./proc ./sys
x86_BOOT_DIRS=./boot
x86_SRCS=$(shell find $(x86_KERNEL_DIRS) -name "*.c" -or -name "*.s")
x86_SRCS_BOOT=$(shell find $(x86_BOOT_DIRS) -name "*.c" -or -name "*.s")
x86_OBJS=$(x86_SRCS:%=$(OBJ_DIR)/%.o)
x86_OBJS_BOOT=$(x86_SRCS_BOOT:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/x86/loof.img

$(BUILD_DIR)/x86/loof.img: $(BUILD_DIR)/x86/kernel $(BUILD_DIR)/x86/boot
	# Formatting
	mkdir $(BUILD_DIR)/x86/iso
	cp $^ $(BUILD_DIR)/x86/iso/
	mkisofs -V 'loofOS' $(FSFLAGS) -o $(BUILD_DIR)/x86/loof.iso -b boot $(BUILD_DIR)/x86/iso
	# Cleanup
	rm -rf $(BUILD_DIR)/x86/iso

$(BUILD_DIR)/x86/boot: $(x86_OBJS_BOOT)
	mkdir -p $(dir $@)
	$(LD) --oformat binary $(LDFLAGS) --script=boot_linker.ld -o $@ $^
	$(LD) --oformat elf_i386 $(LDFLAGS) --script=boot_linker.ld -o $@.o $^

$(BUILD_DIR)/x86/kernel: $(x86_OBJS)
	mkdir -p $(dir $@)
	$(LD) --oformat binary $(LDFLAGS) --script=kernel_linker.ld -o $@ $^
	$(LD) --oformat elf_i386 $(LDFLAGS) --script=kernel_linker.ld -o $@.o $^
#	x86_64-elf-objcopy -O binary --adjust-vma 0x100000 $@ $@

# Build C files
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Build assembly files
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) -o $@ $< -f elf32

