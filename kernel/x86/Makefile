LD=ld.lld
AS=nasm
CC=clang
LDFLAGS=-nostdlib -O0 --format=elf --nmagic --gc-sections --gdb-index
CFLAGS=-nostdlib -O0 -ffreestanding -m32 -I./ -march=i386 --target=i386-linux-gnu -g
ASFLAGS=-O0 -g

OBJ_DIR=../../obj/kernel
BUILD_DIR=../../build

SRCS=$(shell find . -name "*.s" -or -name "*.c")
OBJS=$(SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/kernel

$(BUILD_DIR)/kernel: $(OBJS)
	$(LD) $(LDFLAGS) --oformat binary --script=kernel_linker.ld -o $@ $^
	$(LD) $(LDFLAGS) --oformat elf_i386 --script=kernel_linker.ld -o $@.o $^

$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -f elf32 -o $@ $<

$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<


