CC=gcc
AS=as
CFLAGS=-Wall -O2 -ffreestanding -nostdlib
BUILD_DIR=build
OBJ_DIR=obj
SRC_DIR=src
ARCH_DIR=$(SRC_DIR)/arch

# Arch specific build info
ARM_SRCS=$(shell find $(ARCH_DIR)/arm -name *.c -or -name *.s)
ARM_OBJS=$(ARM_SRCS:%=$(OBJ_DIR)/%.o)


all:
	@echo "Specify an arch:\n- arm"

arm: $(BUILD_DIR)/arm/kernel.img

# Build kernel image
$(BUILD_DIR)/arm/kernel.img: $(ARM_OBJS)
	mkdir -p $(dir $@)
	ld -o $@ $^ -T $(ARCH_DIR)/arm/linker.ld

# Build C files
$(OBJ_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $< -Iinclude

# Build assembly files
$(OBJ_DIR)/%.s.o: %.s
	mkdir -p $(dir $@)
	$(AS) -o $@ $<

.PHONY: clean
clean:
	rm -r $(BUILD_DIR) $(OBJ_DIR)
