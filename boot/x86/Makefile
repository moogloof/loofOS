LD=ld.lld
AS=nasm
CC=clang
LDFLAGS=-nostdlib --format=elf -O0 --nmagic --gdb-index
CFLAGS=-nostdlib -lgcc -Wall -O0 -ffreestanding -I./ -m16 -g
ASFLAGS=-O0

OBJ_DIR=../../obj
BUILD_DIR=../../build

SRCS=$(shell find . \( -name "*.s" -or -name "*.c" \) -and \( ! -name "boot.s" -and ! -name "mbr.s" \))
OBJS=$(SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/mbr $(BUILD_DIR)/boot # $(BUILD_DIR)/stage2.sys

$(BUILD_DIR)/mbr: $(OBJS) $(OBJ_DIR)/mbr.s.o
	$(LD) $(LDFLAGS) --oformat binary --script=boot_linker.ld -o $@ $^

$(BUILD_DIR)/boot: $(OBJS) $(OBJ_DIR)/boot.s.o
	$(LD) $(LDFLAGS) --oformat binary --script=boot_linker.ld -o $@ $^

$(BUILD_DIR)/stage2.sys: $(OBJS)
	$(LD) $(LDFLAGS) --oformat binary --script=boot_linker.ld -o $@ $^

$(OBJ_DIR)/%.c.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.s.o: %.s
	$(AS) -o $@ $< -f elf32
