LD=ld.lld
AS=nasm
CC=clang
LDFLAGS=-nostdlib -O3 --format=elf --nmagic --gc-sections --gdb-index
CFLAGS=-nostdlib -Wall -Werror -Wpedantic -O3 -ffreestanding -m32 -I./ -I../../common -march=i386 --target=i386-linux-gnu -g
ASFLAGS=-O0 -g

OBJ_DIR=../../obj/boot
BUILD_DIR=../../build

SRCS=$(shell find . \( -name "*.s" -or -name "*.c" \) -and \( ! -name "boot.s" -and ! -name "mbr.s" \)) $(shell find ../../common \( -name "*.s" -or -name "*.c" \))
OBJS=$(SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/mbr $(BUILD_DIR)/boot

$(BUILD_DIR)/mbr: $(OBJS) $(OBJ_DIR)/mbr.s.o
	@printf "LD :: mbr\r\n"
	@$(LD) $(LDFLAGS) --oformat binary --script=boot_linker.ld -o $@ $^
	@$(LD) $(LDFLAGS) --oformat elf_i386 --script=boot_linker.ld -o $@.o $^

$(BUILD_DIR)/boot: $(OBJS) $(OBJ_DIR)/boot.s.o
	@printf "LD :: boot\r\n"
	@$(LD) $(LDFLAGS) --oformat binary --script=boot_linker.ld -o $@ $^
	@$(LD) $(LDFLAGS) --oformat elf_i386 --script=boot_linker.ld -o $@.o $^

$(OBJ_DIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) -f elf32 -o $@ $<
	@printf "AS :: $<\r\n"

$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@printf "CC :: $<\r\n"
