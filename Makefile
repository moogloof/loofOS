CC=gcc
AS=as
CFLAGS=-Wall -O0 -ffreestanding -nostdlib -nostartfiles
ASFLAGS=-Iinclude -O0
BUILD_DIR=build
OBJ_DIR=obj
ARCH_DIR=arch

all:
	@echo "Specify an arch:\n- arm\n- x86"

x86:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(ARCH_DIR)/x86

x86_boot:
	rm $(BUILD_DIR)/x86/boot
	$(MAKE) -C $(ARCH_DIR)/x86
x86_kernel:
	rm $(BUILD_DIR)/x86/kernel
	$(MAKE) -C $(ARCH_DIR)/x86

arm:
	$(MAKE) -C $(ARCH_DIR)/arm

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)
