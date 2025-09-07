#include "stm32f10x.h"

void delay_us(uint32_t xus)
{
    /* 72Mhz, please update if frequency is different */
    SysTick->LOAD = 72 * xus;

    /* Clear load value */
    SysTick->VAL = 0;

    /* Select clk source as processor and enable it */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
    {}

    /* Disable systick */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


void delay_ms(uint32_t xms)
{
    delay_us(xms * 1000);
}

/* Systick->LOAD is only 24-bits so need to reuse ms delay for s */
void delay_s(uint32_t xs)
{
    for(uint32_t i = 0; i <= 10 * xs ; i++)
    {
        delay_ms(100);
    }
}
