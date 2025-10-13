#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdbool.h>
#include <string.h>

#define UART_BUFFER_SIZE 52

struct
{
    uint8_t uart_buffer[UART_BUFFER_SIZE];
    uint32_t tx_counter;
    uint32_t rx_counter;
} uart_stream;

/* PB10 TX PB11 RX */
void uart_init(void)
{
    /* Enable clock for GPIO */
    RCC->APB2ENR |= RCC_APB2Periph_GPIOB;

    /* PB10 push-pull TX */
    uint32_t temp = GPIOB->CRH;
    
    temp &= 0xfffff0ff;
    temp |= 0x00000900;
    GPIOB->CRH = temp;

    /* PB11 floating-in RX */
    temp &= 0xffff0fff;
    temp |= 0x00004000;
    GPIOB->CRH = temp;

    /* Enable clock for UART3 */
    RCC->APB1ENR |= RCC_APB1Periph_USART3;

    /* Config UART3 */
    USART3->BRR = 0x0273;                       // 115200 baud rate
    USART3->CR1 &= ~USART_CR1_M;                // 8-bits data
    USART3->CR1 &= ~USART_CR1_PCE;              // No verification
    USART3->CR1 |= USART_Mode_Rx|USART_Mode_Tx; // TX/RX
    USART3->CR2 &= ~USART_CR2_STOP;             // 1 stop bit

    /* Enable UART3 and its RX interrupt */
    USART3->CR1 |= USART_CR1_UE | USART_CR1_RXNEIE;

    /* Enable USART3 interrupt on NVIC, ignoring priority */
    NVIC->ISER[((uint32_t)(USART3_IRQn) >> 5)] = (1 << ((uint32_t)(USART3_IRQn) & 0x1F));

    /* Initialise usart buffer */
    uart_stream.rx_counter = 0;
    uart_stream.tx_counter = 0;

    memset(&uart_stream.uart_buffer[0], 0, UART_BUFFER_SIZE);
}


void uart_send(uint8_t data)
{
    while (!(USART3->SR & USART_SR_TXE)); // TX register empty
    USART3->DR = data;                    // write data register
    while (!(USART3->SR & USART_SR_TC));  // TX complete
}


void USART3_IRQHandler(void)
{
    while(USART3->SR & USART_SR_RXNE)
    {
        uart_stream.uart_buffer[uart_stream.rx_counter] = USART3->DR;
        uart_stream.rx_counter++;
        uart_stream.rx_counter = uart_stream.rx_counter % UART_BUFFER_SIZE;
    }
    /* Clear pending */
    NVIC->ICPR[((uint32_t)(USART3_IRQn) >> 5)] = (1 << ((uint32_t)(USART3_IRQn) & 0x1F));
    return;
}


/* Test function */
void usart_tx_test(void)
{
    while (uart_stream.rx_counter != uart_stream.tx_counter)
    {
        uart_send(uart_stream.uart_buffer[uart_stream.tx_counter]);
        uart_stream.tx_counter++;
        uart_stream.tx_counter = uart_stream.tx_counter % UART_BUFFER_SIZE;
    }
}