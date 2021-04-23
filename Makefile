CC=gcc
AS=as
CFLAGS=-Wall -O2 -ffreestanding -nostdlib -nostartfiles
ASFLAGS=-Iinclude
BUILD_DIR=build
OBJ_DIR=obj
SRC_DIR=src
ARCH_DIR=$(SRC_DIR)/arch
LIBC_DIR=$(SRC_DIR)/libc
CUR_ARCH=arm

# Set platform specific compilers and linkers
ifeq ($(shell uname),Darwin)
	CC=x86_64-elf-gcc
	AS=x86_64-elf-as
	CUR_ARCH=x86
endif

# Standard library specific build info
LIBC_SRCS=$(shell find $(LIBC_DIR) -name *.c -or -name *.s)
LIBC_OBJS=$(LIBC_SRCS:%=$(OBJ_DIR)/%.o)

# Arch specific build info
ARM_SRCS=$(shell find $(ARCH_DIR)/arm -name *.c -or -name *.s)
ARM_OBJS=$(ARM_SRCS:%=$(OBJ_DIR)/%.o)

# x86_64 specific build info
x86_SRCS=$(shell find $(ARCH_DIR)/x86 -name *.c -or -name *.s)
x86_OBJS=$(x86_SRCS:%=$(OBJ_DIR)/%.o)

all:
	@echo "Specify an arch:\n- arm\n- x86_64"

arm: $(BUILD_DIR)/arm/kernel7.img

x86_64: $(BUILD_DIR)/x86_64/kernel.img

# Build kernel image
$(BUILD_DIR)/arm/kernel7.img: $(ARM_OBJS) $(LIBC_OBJS)
	mkdir -p $(dir $@)
	ld -o $@ $^ -T $(ARCH_DIR)/arm/linker.ld

$(BUILD_DIR)/x86_64/kernel.img: $(x86_OBJS)
	mkdir -p $(dir $@)
	x86_64-elf-ld -o $@ $^ -T$(ARCH_DIR)/x86/linker.ld

# Build C files
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $< -Iinclude -I$(ARCH_DIR)/$(CUR_ARCH)

# Build assembly files
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) -o $@ $< $(ASFLAGS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR) $(OBJ_DIR)
