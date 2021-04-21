#ifndef IRQ_DRIVER_H
#define IRQ_DRIVER_H

// IRQ init
void irq_init();
// IRQ handler
void _irq_handler();
// IRQ enable
void irq_enable();
// IRQ disable
void irq_disable();

#endif
