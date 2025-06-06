LD=ld.lld
AS=nasm
CC=clang
LDFLAGS=-nostdlib -O0 --format=elf --nmagic --gc-sections --gdb-index
CFLAGS=-nostdlib -O0 -ffreestanding -m32 -I./ -march=i386 --target=i386-linux-gnu -g
ASFLAGS=-O0 -g

OBJ_DIR=../../obj/system
BUILD_DIR=../../build/sys/system

SRCS=$(shell find . -name "*.s" -or -name "*.c")
OBJS=$(SRCS:%=$(OBJ_DIR)/%.o)
TARGETS_PROG=$(patsubst %.c,%,$(patsubst %.s,%,$(SRCS)))
TARGETS=$(TARGETS_PROG:%=$(BUILD_DIR)/%)

all: $(TARGETS)

$(BUILD_DIR)/%: %
	$(LD) $(LDFLAGS) --oformat binary --script=linker.ld -o $@ $<

$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -f elf32 -o $@ $<

$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
