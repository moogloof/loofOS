LD=ld.lld
CC=clang
AS=nasm
LDFLAGS=--oformat binary --format=elf -O0 --nmagic -nostdlib --gc-sections --gdb-index --script=kernel_linker.ld
CFLAGS=-Wall -O0 -ffreestanding -I./ -m32 -nostdlib -mgeneral-regs-only -march=i386 --target=i386-unknown-linux-gnu -g
ASFLAGS=-O0 -f bin
BUILD_DIR=../../build
OBJ_DIR=../../obj
LIBC_DIR=libc

# Standard library specific build info
LIBC_SRCS=$(shell find $(LIBC_DIR) -name "*.c" -or -name "*.s")
LIBC_OBJS=$(LIBC_SRCS:%=$(OBJ_DIR)/%.o)

# x86 specific build info
x86_SRCS=$(shell find . -name "*.c" -or -name "*.s" ! -name "*boot.s")
x86_OBJS=$(x86_SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/x86/loof.img

$(BUILD_DIR)/x86/loof.img: $(BUILD_DIR)/x86/kernel $(BUILD_DIR)/x86/boot
	dd if=/dev/zero of=$@ bs=1024 count=1440
	dd seek=0 if=$(BUILD_DIR)/x86/boot of=$@ bs=512 conv=notrunc
	dd seek=5 if=$(BUILD_DIR)/x86/kernel of=$@ bs=512 conv=notrunc
	# Formatting
	mkdir $(BUILD_DIR)/x86/iso
	cp $@ $(BUILD_DIR)/x86/iso/
	mkisofs -quiet -V 'loofOS' -input-charset iso8859-1 -o $(BUILD_DIR)/x86/loof.iso -b loof.img $(BUILD_DIR)/x86/iso
	# Cleanup
	rm -rf $(BUILD_DIR)/x86/iso

$(BUILD_DIR)/x86/boot:
	mkdir -p $(dir $@)
	$(AS) -o $@ boot/boot.s $(ASFLAGS)

$(BUILD_DIR)/x86/kernel: $(x86_OBJS)
	mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $^
	$(LD) --oformat elf_i386 --format=elf -O0 --nmagic -nostdlib --gc-sections --gdb-index --script=kernel_linker.ld -o $@.o $^
#	x86_64-elf-objcopy -O binary --adjust-vma 0x100000 $@ $@

# Build C files
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Build assembly files
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) -o $@ $< -f elf32

