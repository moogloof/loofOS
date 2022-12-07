#ifndef MINI_UART_DRIVER_H
#define MINI_UART_DRIVER_H

// Initializer function
void mini_uart_init();
// Send char function
void mini_uart_sendc(char);
// Send string function
void mini_uart_sendstr(char*);
// Receive char function
char mini_uart_recv();

#endif
