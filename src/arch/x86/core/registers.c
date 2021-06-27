#include <core/registers.h>

// Save current registers to the register set passed
void save_registers(register_set* set) {
	// Save general registers
	__asm__("mov %%eax, %0" : "=m"(set->eax));
	__asm__("mov %%ebx, %0" : "=m"(set->ebx));
	__asm__("mov %%ecx, %0" : "=m"(set->ecx));
	__asm__("mov %%edx, %0" : "=m"(set->edx));
	__asm__("mov %%esi, %0" : "=m"(set->esi));
	__asm__("mov %%edi, %0" : "=m"(set->edi));
	__asm__("mov %%esp, %0" : "=m"(set->esp));
	__asm__("mov %%ebp, %0" : "=m"(set->ebp));

	// Save instruction pointer
	__asm__("call get_eip\n"
		"get_eip:\n"
		"pop %0" : "=m"(set->eip));

	// Save flags
	__asm__("pushf\n"
		"pop %0\n" : "=r"(set->flags));
}

// Load passed register set to current register set
void load_registers(register_set* set) {
	// Load general registers
	__asm__("mov %0, %%eax" : : "m"(set->eax));
	__asm__("mov %0, %%ebx" : : "m"(set->ebx));
	__asm__("mov %0, %%ecx" : : "m"(set->ecx));
	__asm__("mov %0, %%edx" : : "m"(set->edx));
	__asm__("mov %0, %%esi" : : "m"(set->esi));
	__asm__("mov %0, %%edi" : : "m"(set->edi));
	__asm__("mov %0, %%esp" : : "m"(set->esp));
	__asm__("mov %0, %%ebp" : : "m"(set->ebp));

	// Jump to the saved eip
	__asm__("jmp %0" : : "m"(set->eip));

	// Load the flags
	__asm__("push %0\n"
		"popf" : : "m"(set->flags));
}
