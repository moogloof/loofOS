LD=ld.lld
CC=clang
AS=nasm
LDFLAGS=--format=elf -O0 --nmagic -nostdlib --gc-sections --gdb-index
CFLAGS=-Wall -O0 -ffreestanding -I./ -m32 -nostdlib -mgeneral-regs-only -march=i386 --target=i386-unknown-linux-gnu -g
ASFLAGS=-O0 -f bin
BUILD_DIR=../../build
OBJ_DIR=../../obj
LIBC_DIR=libc
FSFLAGS=-no-emul-boot -boot-load-size 4 -boot-info-table

# Standard library specific build info
LIBC_SRCS=$(shell find $(LIBC_DIR) -name "*.c" -or -name "*.s")
LIBC_OBJS=$(LIBC_SRCS:%=$(OBJ_DIR)/%.o)

# x86 specific build info
x86_BOOT_DIR=./boot
x86_SRCS=$(shell find . \( -name "*.c" -or -name "*.s" \) -not \( -path "$(x86_BOOT_DIR)*" -prune \))
x86_SRCS_BOOT=$(shell find $(x86_BOOT_DIR) -name "*.c" -or -name "*.s")
x86_OBJS=$(x86_SRCS:%=$(OBJ_DIR)/%.o)
x86_OBJS_BOOT=$(x86_SRCS_BOOT:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/x86/loof.iso

$(BUILD_DIR)/x86/loof.iso: $(BUILD_DIR)/x86/kernel $(BUILD_DIR)/x86/boot
	# Formatting
	mkdir -p $(BUILD_DIR)/x86/iso
	cp -R ../../system $(BUILD_DIR)/x86/iso
	cp $^ $(BUILD_DIR)/x86/iso/system
	# TODO: -G option for general boot do it DO IT
	mkisofs -V "loofOS" $(FSFLAGS) -o $@ -b system/boot $(BUILD_DIR)/x86/iso
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

