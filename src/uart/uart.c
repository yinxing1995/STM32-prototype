#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

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

    /* Enable UART3 */
    USART3->CR1 |= USART_CR1_UE;
}


void uart_send(uint8_t data)
{
    while (!(USART3->SR & USART_SR_TXE)); // TX register empty
    USART3->DR = data;                    // write data register
    while (!(USART3->SR & USART_SR_TC));  // TX complete
}


//WIP
void USART3_IRQHandler(void)
{
    return;
}
