#include "uart.h"
#include "delay.h"


int main(void)
{
    uart_init();
    for(;;)
    {
        delay_s(1);
        __asm("nop");
        uart_send('a');
    }
    return 0;
}
