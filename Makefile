BUILD_DIR=build
OBJ_DIR=obj
KERNEL_DIR=kernel
BOOT_DIR=boot
SYS_PROG_DIR=system
FS_PROG=hdiutil create
FS_FLAGS=-format UDTO -layout NONE -ov -fs ExFAT
ISO_PROG=mkisofs
ISO_FLAGS=-no-emul-boot -boot-info-table -boot-load-size 4 -V "loofOS"

all:
	@echo "Specify an arch:\n- arm\n- x86"

x86:
	@rm -rf $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)
	@# Prepare the directory
	@mkdir -p $(BUILD_DIR)/sys/system
	@mkdir $(BUILD_DIR)/cdrom
	@# Make the boot
	@printf "Compiling bootloader\r\n"
	@$(MAKE) -C $(BOOT_DIR)/$@
	@# Make the kernel
	@printf "Compiling kernel\r\n"
	@$(MAKE) -C $(KERNEL_DIR)/$@
	@# Make the system programs
	@printf "Compiling system programs\r\n"
	@$(MAKE) -C $(SYS_PROG_DIR)
	@# Pack into one iso
	@# Make 16 MB iso
	@printf "Creating ISO\r\n"
	@dd if=/dev/zero of=$(BUILD_DIR)/loof.iso bs=16M count=1
	@# Copy in MBR and stage2 boot
	@printf "Moving bootloader\r\n"
	@cp $(BUILD_DIR)/boot $(BUILD_DIR)/cdrom
	@$(ISO_PROG) $(ISO_FLAGS) -b boot -o $(BUILD_DIR)/boot.cdrom $(BUILD_DIR)/cdrom
	@dd if=$(BUILD_DIR)/mbr of=$(BUILD_DIR)/boot.cdrom conv=notrunc bs=512 count=64
	@dd if=$(BUILD_DIR)/boot.cdrom of=$(BUILD_DIR)/loof.iso conv=notrunc bs=512 count=256
	@# Copy in kernel
	@printf "Moving kernel\r\n"
	@cp $(BUILD_DIR)/kernel $(BUILD_DIR)/sys/system
	@# Copy in the partition
	@printf "Moving system programs and padding\r\n"
	@$(FS_PROG) $(FS_FLAGS) -srcfolder $(BUILD_DIR)/sys $(BUILD_DIR)/loof
	@dd if=$(BUILD_DIR)/loof.cdr of=$(BUILD_DIR)/loof.iso conv=notrunc bs=512 seek=256

arm:
	$(MAKE) -C $(ARCH_DIR)/arm

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)
