BUILD_DIR=build
OBJ_DIR=obj
KERNEL_DIR=kernel
BOOT_DIR=boot

all:
	@echo "Specify an arch:\n- arm\n- x86"

x86:
	mkdir -p $(BUILD_DIR)/$@
	mkdir -p $(OBJ_DIR)/$@
	# Make the boot
	$(MAKE) -C $(BOOT_DIR)/$@
	# Make the kernel
	$(MAKE) -C $(KERNEL_DIR)/$@

arm:
	$(MAKE) -C $(ARCH_DIR)/arm

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)
