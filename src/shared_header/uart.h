#ifndef UART_H
#define UART_H
#include <stdint.h>

void uart_init(void);

void uart_send(uint8_t data);

void usart_tx_test(void);

#endif
