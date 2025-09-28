#include "uart.h"
#include "delay.h"
#include "watchpoint.h"


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
    volatile uint32_t debug_var = 0x00000000;
    debug_dwt_init();
    debug_dwt_set(&debug_var);
    __asm("isb 0");
    debug_var = 0xffffffff;
    for(;;)
    {

    }
    return 0;
}
