void kernel_main() {
	// Load and init the gdt
	init_gdt();
	load_gdt();

	// Stuff
	while (1) {}
}
