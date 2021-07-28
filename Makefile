CC=gcc
AS=as
CFLAGS=-Wall -O0 -ffreestanding -nostdlib -nostartfiles
ASFLAGS=-Iinclude -O0
BUILD_DIR=build
OBJ_DIR=obj
ARCH_DIR=arch
LIBC_DIR=libc
CUR_ARCH=arm

# Standard library specific build info
LIBC_SRCS=$(shell find $(LIBC_DIR) -name *.c -or -name *.s)
LIBC_OBJS=$(LIBC_SRCS:%=$(OBJ_DIR)/%.o)

# Arch specific build info
ARM_SRCS=$(shell find $(ARCH_DIR)/arm -name *.c -or -name *.s)
ARM_OBJS=$(ARM_SRCS:%=$(OBJ_DIR)/%.o)

all:
	@echo "Specify an arch:\n- arm\n- x86_64"

arm: $(BUILD_DIR)/arm/kernel7.img

x86_64:
	$(MAKE) -C $(ARCH_DIR)/x86

x86_64_boot:
	rm $(BUILD_DIR)/x86_64/boot
	$(MAKE) -C $(ARCH_DIR)/x86
x86_64_kernel:
	rm $(BUILD_DIR)/x86_64/kernel
	$(MAKE) -C $(ARCH_DIR)/x86

# Build kernel image
$(BUILD_DIR)/arm/kernel7.img: $(ARM_OBJS) $(LIBC_OBJS)
	mkdir -p $(dir $@)
	ld -o $@ $^ -T $(ARCH_DIR)/arm/linker.ld

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
