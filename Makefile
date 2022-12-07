BUILD_DIR=build
OBJ_DIR=obj
KERNEL_DIR=kernel
BOOT_DIR=boot
FS_PROG=mkisofs
FS_FLAGS=-no-emul-boot -boot-load-size 4 -boot-info-table

all:
	@echo "Specify an arch:\n- arm\n- x86"

x86:
	rm -rf $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OBJ_DIR)
	# Make the boot
	$(MAKE) -C $(BOOT_DIR)/$@
	# Make the kernel
	#$(MAKE) -C $(KERNEL_DIR)/$@
	# Prepare the directory
	mkdir $(BUILD_DIR)/sys
	mv $(BUILD_DIR)/boot $(BUILD_DIR)/sys
	#mv $(BUILD_DIR)/stage2.sys $(BUILD_DIR)/sys
	# Pack into one iso
	mkisofs $(FS_FLAGS) -b boot -V "loofOS" -hide boot -o $(BUILD_DIR)/loof.iso $(BUILD_DIR)/sys
	dd if=$(BUILD_DIR)/sys/boot of=$(BUILD_DIR)/loof.iso conv=notrunc

arm:
	$(MAKE) -C $(ARCH_DIR)/arm

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)
