#include "watchpoint.h"
#include "stm32f10x.h"

#define DWT_CTRL        *(volatile uint32_t *)0xe0001000
#define DWT_COMP_0      *(volatile uint32_t *)0xe0001020
#define DWT_FUNCTION_0  *(volatile uint32_t *)0xe0001028

void debug_dwt_init(void)
{
    /* Daddr RW */
    DWT_FUNCTION_0 |= 0x000000007;

    /* Enable debug monitor exception */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_MON_EN_Msk;
}

void debug_dwt_set(void *addr)
{
    DWT_COMP_0 = (uint32_t)addr;
}