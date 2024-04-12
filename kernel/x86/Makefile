LD=ld.lld
AS=nasm
CC=clang
LDFLAGS=-nostdlib -O3 --format=elf --nmagic --gc-sections --gdb-index
CFLAGS=-nostdlib -Wall -Werror -Wpedantic -O3 -ffreestanding -m32 -I./ -I../../common -march=i386 --target=i386-linux-gnu -g
ASFLAGS=-O0 -g

OBJ_DIR=../../obj/kernel
BUILD_DIR=../../build

SRCS=$(shell find . -name "*.s" -or -name "*.c") $(shell find ../../common \( -name "*.s" -or -name "*.c" \))
OBJS=$(SRCS:%=$(OBJ_DIR)/%.o)

all: $(BUILD_DIR)/kernel

$(BUILD_DIR)/kernel: $(OBJS)
	@printf "Linking KERNEL\r\n"
	@$(LD) $(LDFLAGS) --oformat binary --script=kernel_linker.ld -o $@ $^
	@$(LD) $(LDFLAGS) --oformat elf_i386 --script=kernel_linker.ld -o $@.o $^

$(OBJ_DIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) -f elf32 -o $@ $<
	@printf "AS :: $<\r\n"

$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@printf "CC :: $<\r\n"
