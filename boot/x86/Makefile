LD=ld.lld
AS=nasm
CC=clang
LDFLAGS=-nostdlib --format=elf -O0 --nmagic --gc-sections --gdb-index
CFLAGS=-nostdlib -Wall -O0 -ffreestanding -I./ -m16 -g -march=i386 --target=i386-unknown-linux-gnu
ASFLAGS=-O0 -g

OBJ_DIR=../../obj
BUILD_DIR=../../build

SRCS=$(shell find . \( -name "*.s" -or -name "*.c" \) -and \( ! -name "boot.s" -and ! -name "mbr.s" \))
OBJS=$(SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/mbr $(BUILD_DIR)/boot

$(BUILD_DIR)/mbr: $(OBJS) $(OBJ_DIR)/mbr.s.o
	$(LD) --oformat binary $(LDFLAGS) --script=boot_linker.ld -o $@ $^
	$(LD) --oformat elf_i386 $(LDFLAGS) --script=boot_linker.ld -o $@.o $^

$(BUILD_DIR)/boot: $(OBJS) $(OBJ_DIR)/boot.s.o
	$(LD) $(LDFLAGS) --oformat binary --script=boot_linker.ld -o $@ $^
	$(LD) $(LDFLAGS) --oformat elf_i386 --script=boot_linker.ld -o $@.o $^

$(OBJ_DIR)/%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.s.o: %.s
	$(AS) $(ASFLAGS) -o $@ $< -f elf32
