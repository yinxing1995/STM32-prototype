#include "uart.h"
#include "delay.h"
#include "watchpoint.h"
#include "rtc_sleep.h"

uint32_t debug_var = 0x1;
char debug_char = 'H';

int main(void)
{
    uart_init();
    rtc_init();

#if 0
    for(;;)
    {
        enter_low_power(3);
        __asm("nop");
        uart_send(debug_char);
    }
#endif

#if 0
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

#if 0
    for(;;)
    {
        delay_ms(10);
        //__asm("nop");
        usart_tx_test();
    }
#endif
    while(1);
}
