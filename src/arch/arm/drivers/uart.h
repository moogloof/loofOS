#ifndef UART_DRIVER_H
#define UART_DRIVER_H

void uart_init();
void uart_sendc(char);
void uart_sendstr(char*);
char uart_recv();

#endif
