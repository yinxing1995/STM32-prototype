#include "uart.h"
#include "delay.h"
#include "watchpoint.h"

uint32_t debug_var = 0x0;

int main(void)
{
#if 0
    uart_init();  
    for(;;)
    {
        delay_s(1);
        __asm("nop");
        uart_send('a');
    }
#endif

#if 0
    uart_init();

    debug_dwt_init();

    debug_dwt_set(&debug_var);
    debug_var = 0xffffffff;
    __asm("dsb 0");

    for(;;)
    {
        delay_s(1);
        __asm("nop");
        uart_send('a');
    }
#endif

#if 1
    uart_init();
    for(;;)
    {
        delay_ms(10);
        //__asm("nop");
        usart_tx_test();
    }
#endif
}
