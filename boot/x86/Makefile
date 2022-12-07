LD=ld.lld
AS=nasm
CC=clang
LDFLAGS=-nostdlib --format=elf -O0 --nmagic --gdb-index
CFLAGS=-nostdlib -lgcc -Wall -O0 -ffreestanding -I./ -m16 -g
ASFLAGS=-O0

OBJ_DIR=../../obj
BUILD_DIR=../../build

SRCS=$(shell find . \( -name "*.s" -or -name "*.c" \) -and ! -name "boot.s")
OBJS=$(SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/boot # $(BUILD_DIR)/stage2.sys

$(BUILD_DIR)/boot:
	$(AS) $(ASFLAGS) -f bin -o $@ boot.s

$(BUILD_DIR)/stage2.sys: $(OBJS)
	$(LD) $(LDFLAGS) --oformat binary --script=boot_linker.ld -o $@ $^
	$(LD) $(LDFLAGS) --oformat elf_i386 --script=boot_linker.ld -o $@.o $^

$(OBJ_DIR)/%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.s.o: %.s
	$(AS) -o $@ $< -f elf32
