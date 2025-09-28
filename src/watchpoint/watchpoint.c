#include "watchpoint.h"
#include "stm32f10x.h"

#define DWT_CTRL        *(volatile uint32_t *)0xe0001000
#define DWT_COMP_0      *(volatile uint32_t *)0xe0001020
#define DWT_MASK_0      *(volatile uint32_t *)0xe0001024
#define DWT_FUNCTION_0  *(volatile uint32_t *)0xe0001028

void debug_dwt_init(void)
{
    DWT_FUNCTION_0 = 0x0;

    /* Unlock access to debug halt control status register */
    CoreDebug->DHCSR = (0xa05f) << 16;

    /* Disable halt debug */
    CoreDebug->DHCSR &= ~CoreDebug_DHCSR_C_DEBUGEN_Msk;

    SCB->SHP[8] = 0x11;

    /* Enable debug monitor exception */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_MON_EN_Msk | CoreDebug_DEMCR_TRCENA_Msk;

    __asm("isb 0");
}

void debug_dwt_set(void *addr)
{
    DWT_COMP_0 = (uint32_t)addr;
    DWT_MASK_0 = 0;
    
    /* Daddr RW */
    DWT_FUNCTION_0 |= 0x000000007;

    __asm("isb 0");
}